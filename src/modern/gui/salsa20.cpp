#include "salsa20.h"
#include "../algos/salsa20.h"

Salsa20Box::Salsa20Box() 
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,    1, 1, 2);

    m_KeyGrid.attach(m_KeywordLabel, 1, 2);
    m_KeyGrid.attach(m_KeywordEntry, 2, 2);

    m_KeyGrid.attach(m_NonceLabel, 1, 3);
    m_KeyGrid.attach(m_NonceEntry, 2, 3);

    m_KeyGrid.attach(m_ModeLabel, 3, 1, 2);
    m_KeyGrid.attach(m_16ByteLabel, 3, 2);
    m_KeyGrid.attach(m_16ByteRadioButton, 4, 2);
    m_KeyGrid.attach(m_32ByteLabel, 3, 3);
    m_KeyGrid.attach(m_32ByteRadioButton, 4, 3);

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
        sigc::mem_fun(*this, Salsa20Box::changed_keyword_text)
    );

    m_NonceLabel.set_text("Nonce ");
    m_refNonceBuffer = m_NonceEntry.get_buffer();
    m_NonceEntry.set_text("12345678");
    m_NonceEntry.signal_changed().connect(
        sigc::mem_fun(*this, Salsa20Box::changed_nonce_text)
    );

    m_ModeLabel.set_text("Mode ");
    m_16ByteLabel.set_text("16 byte");
    m_32ByteLabel.set_text("32 byte");
    m_16ByteRadioButton.join_group(m_32ByteRadioButton);
    m_16ByteRadioButton.set_active(true);

    m_16ByteRadioButton.signal_toggled().connect(
        sigc::mem_fun(*this, Salsa20Box::changed_mode)
    );

    m_PlainTextLabel.set_text("Plain Message (in base64)");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("AAAAAAAAAAAA");
    m_refPlainTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, Salsa20Box::changed_message_text)
    );
    m_PlainTextView.set_buffer(m_refPlainTextBuffer);
    m_PlainTextView.set_wrap_mode(Gtk::WRAP_WORD);

    m_EncryptedTextLabel.set_text("Encrypted Message (in base64)");
    m_refEncryptedTextBuffer = Gtk::TextBuffer::create();
    m_refEncryptedTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, Salsa20Box::changed_encrypted_text)
    );
    m_EncryptedTextView.set_buffer(m_refEncryptedTextBuffer);
    m_EncryptedTextView.set_wrap_mode(Gtk::WRAP_WORD);

    encrypt();

    logger_attach(&logger, this);
}
    


void Salsa20Box::changed_message_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        
        if (!logger.has_errors)
            encrypt();

        m_ignoreTextInput = false;
    }
}

void Salsa20Box::changed_encrypted_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        
        if (!logger.has_errors)
            decrypt();

        m_ignoreTextInput = false;
    }
}

void Salsa20Box::changed_mode()
{
    changed_keyword_text();
}

void Salsa20Box::changed_nonce_text()
{
    changed_keyword_text();
}

void Salsa20Box::changed_keyword_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        validate();
        
        if (!logger.has_errors)
            encrypt();

        m_ignoreTextInput = false;
    }
}

Glib::ustring Salsa20Box::crypt(const Glib::ustring& buffer)
{
    auto key_string = m_refKeywordBuffer->get_text();
    auto nonce = m_refNonceBuffer->get_text();
    auto base64_decoded = Glib::Base64::decode(buffer);
    auto num_bytes = base64_decoded.size();

    auto func = m_16ByteRadioButton.get_active() ? &salsa20_crypt16 : &salsa20_crypt32;

    (*func)((u8*)key_string.data(), 
         (u8*)nonce.data(), 
         (u8*)base64_decoded.data(), 
         (u32)num_bytes);

    return Glib::Base64::encode(base64_decoded);
}

void Salsa20Box::encrypt()
{
    auto buffer = m_refPlainTextBuffer->get_text();
    m_refEncryptedTextBuffer->set_text(crypt(buffer));
}


void Salsa20Box::decrypt()
{
    auto buffer = m_refEncryptedTextBuffer->get_text();
    m_refPlainTextBuffer->set_text(crypt(buffer));
}


void Salsa20Box::validate()
{
    logger_clear(&logger);

    int num_nonce_bytes = m_refNonceBuffer->get_bytes();

    if (num_nonce_bytes < 8)
    {
        logger_format_error(&logger, 
            "Not enough bytes in the nonce (%i). Must be at least 8.", num_nonce_bytes);
    }
    // else if (num_nonce_bytes > 8)
    // {
    //     logger_format_error(&logger, 
    //         "Too many bytes in the nonce (%i). Must be 8.", num_nonce_bytes);
    // }

    int required_num_bytes = m_16ByteRadioButton.get_active() ? 16 : 32;
    int num_keyword_bytes = m_refKeywordBuffer->get_bytes();

    if (num_keyword_bytes < required_num_bytes)
    {
        logger_format_error(&logger, 
            "Not enough bytes in the keyword (%i). Must be at least %i.", 
            num_keyword_bytes, required_num_bytes);
    }
    // else if (num_keyword_bytes > required_num_bytes)
    // {
    //     logger_format_error(&logger, 
    //         "Too many bytes in the keyword (%i). Must be %i.", 
    //         num_keyword_bytes, required_num_bytes);
    // }

}

// Salsa20Box::~Salsa20Box() {}
