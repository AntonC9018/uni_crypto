#include "rijndael.h"

RijndaelBox::RijndaelBox() 
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,    1, 1, 2);

    m_KeyGrid.attach(m_KeywordLabel, 1, 2);
    m_KeyGrid.attach(m_KeywordEntry, 2, 2);

    pack_start(m_ModeGrid);

    m_ModeGrid.set_halign(Gtk::ALIGN_CENTER);

    m_ModeGrid.attach(m_ModeLabel, 1, 1, 2);
    m_ModeGrid.attach(m_128ModeLabel, 1, 2);
    m_ModeGrid.attach(m_128ModeRadioButton, 2, 2);
    m_ModeGrid.attach(m_192ModeLabel, 1, 3);
    m_ModeGrid.attach(m_192ModeRadioButton, 2, 3);
    m_ModeGrid.attach(m_256ModeLabel, 1, 4);
    m_ModeGrid.attach(m_256ModeRadioButton, 2, 4);

    pack_start(m_UsageGrid);

    m_UsageGrid.set_halign(Gtk::ALIGN_CENTER);

    m_UsageGrid.attach(m_UsageHeader, 1, 1, 2);
    
    m_UsageGrid.attach(m_PlainTextLabel, 1, 2, 2);
    m_UsageGrid.attach(m_PlainTextView,  1, 3, 2);

    m_UsageGrid.attach(m_EncryptedTextLabel, 1, 4, 2);
    m_UsageGrid.attach(m_EncryptedTextView,  1, 5, 2);


    m_KeyHeader.set_markup("<u><b>Key</b></u>");
    m_KeyHeader.set_margin_top(10);
    m_KeyHeader.set_margin_bottom(10);

    m_KeywordLabel.set_text("Keyword ");
    m_refKeywordBuffer = m_KeywordEntry.get_buffer();
    m_KeywordEntry.set_text("1234567890123456");
    m_KeywordEntry.signal_changed().connect(
        sigc::mem_fun(*this, RijndaelBox::changed_keyword_text)
    );

    m_ModeLabel.set_text("Mode ");
    m_128ModeLabel.set_text("128 bit");
    m_192ModeLabel.set_text("192 bit");
    m_256ModeLabel.set_text("256 bit");
    m_128ModeRadioButton.join_group(m_192ModeRadioButton);
    m_256ModeRadioButton.join_group(m_192ModeRadioButton);
    m_128ModeRadioButton.set_active(true);

    m_128ModeRadioButton.signal_toggled().connect(
        sigc::mem_fun(*this, RijndaelBox::changed_mode)
    );
    m_192ModeRadioButton.signal_toggled().connect(
        sigc::mem_fun(*this, RijndaelBox::changed_mode)
    );
    m_256ModeRadioButton.signal_toggled().connect(
        sigc::mem_fun(*this, RijndaelBox::changed_mode)
    );

    m_PlainTextLabel.set_text("Plain Message (in base64)");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("123456789012345678901234");
    m_refPlainTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, RijndaelBox::changed_message_text)
    );
    m_PlainTextView.set_buffer(m_refPlainTextBuffer);
    m_PlainTextView.set_wrap_mode(Gtk::WRAP_WORD_CHAR);

    m_EncryptedTextLabel.set_text("Encrypted Message (in base64)");
    m_refEncryptedTextBuffer = Gtk::TextBuffer::create();
    m_refEncryptedTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, RijndaelBox::changed_encrypted_text)
    );
    m_EncryptedTextView.set_buffer(m_refEncryptedTextBuffer);
    m_EncryptedTextView.set_wrap_mode(Gtk::WRAP_WORD_CHAR);

    changed_message_text();

    logger_attach(&logger, this);
}
    


void RijndaelBox::changed_message_text()
{
    changed_keyword_text();
}

void RijndaelBox::changed_encrypted_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        auto buffer = m_refEncryptedTextBuffer->get_text();
        validate(buffer);
        
        if (!logger.has_errors)
            decrypt(buffer);

        m_ignoreTextInput = false;
    }
}

void RijndaelBox::changed_mode()
{
    changed_keyword_text();
}

void RijndaelBox::changed_keyword_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        auto buffer = m_refPlainTextBuffer->get_text();
        validate(buffer);
        
        if (!logger.has_errors)
            encrypt(buffer);

        m_ignoreTextInput = false;
    }
}

AES_CYPHER_T RijndaelBox::get_selected_mode()
{
    if (m_128ModeRadioButton.get_active()) return AES_CYPHER_128;
    if (m_192ModeRadioButton.get_active()) return AES_CYPHER_192;
    return AES_CYPHER_256;
}


void RijndaelBox::encrypt(const Glib::ustring& buffer)
{
    auto base64_decoded = Glib::Base64::decode(buffer);

    rijndael_encrypt(
        get_selected_mode(), 
        (u8*)m_refKeywordBuffer->get_text().data(),
        (u8*)base64_decoded.data(),
        base64_decoded.size() - base64_decoded.size() % 16
    );

    m_refEncryptedTextBuffer->set_text(Glib::Base64::encode(base64_decoded));
}


void RijndaelBox::decrypt(const Glib::ustring& buffer)
{
    auto base64_decoded = Glib::Base64::decode(buffer);

    rijndael_decrypt(
        get_selected_mode(), 
        (u8*)m_refKeywordBuffer->get_text().data(),
        (u8*)base64_decoded.data(),
        base64_decoded.size() - base64_decoded.size() % 16
    );

    m_refPlainTextBuffer->set_text(Glib::Base64::encode(base64_decoded));
}

void RijndaelBox::validate(const Glib::ustring& buffer)
{
    logger_clear(&logger);

    int required_num_bytes = rijndael_key_length_in_dwords[get_selected_mode()] * 4;
    int num_keyword_bytes = m_refKeywordBuffer->get_bytes();

    if (num_keyword_bytes < required_num_bytes)
    {
        logger_format_error(&logger, 
            "Not enough bytes in the keyword (%i). Must be at least %i.\n", 
            num_keyword_bytes, required_num_bytes);
    }

    if (buffer.size() < 24)
    {
        logger_format_error(&logger,
            "Not enough bytes in the input (%i). Must be at least 24.\nInformation: the bytes of the input are encrypted/decrypted by 16 at a time, which is at least 24 characters in base64.", (int)buffer.size());
    }
}