#include "trivium.h"
#include "../algos/trivium.h"

TriviumBox::TriviumBox() 
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,    1, 1, 2);

    m_KeyGrid.attach(m_KeywordLabel, 1, 2);
    m_KeyGrid.attach(m_KeywordEntry, 2, 2);

    m_KeyGrid.attach(m_InitializationVectorLabel, 1, 3);
    m_KeyGrid.attach(m_InitializationVectorEntry, 2, 3);

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
    m_KeywordEntry.set_text("1234567890");
    m_KeywordEntry.signal_changed().connect(
        sigc::mem_fun(*this, TriviumBox::changed_keyword_text)
    );

    m_InitializationVectorLabel.set_text("Initialization Vector ");
    m_refInitializationVectorBuffer = m_InitializationVectorEntry.get_buffer();
    m_InitializationVectorEntry.set_text("1234567890");
    m_InitializationVectorEntry.signal_changed().connect(
        sigc::mem_fun(*this, TriviumBox::changed_ivector_text)
    );

    m_PlainTextLabel.set_text("Plain Message (in base64)");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("AAAAAAAAAAAA");
    m_refPlainTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, TriviumBox::changed_message_text)
    );
    m_PlainTextView.set_buffer(m_refPlainTextBuffer);
    m_PlainTextView.set_wrap_mode(Gtk::WRAP_WORD_CHAR);

    m_EncryptedTextLabel.set_text("Encrypted Message (in base64)");
    m_refEncryptedTextBuffer = Gtk::TextBuffer::create();
    m_refEncryptedTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, TriviumBox::changed_encrypted_text)
    );
    m_EncryptedTextView.set_buffer(m_refEncryptedTextBuffer);
    m_EncryptedTextView.set_wrap_mode(Gtk::WRAP_WORD_CHAR);

    changed_message_text();

    logger_attach(&logger, this);
}
    


void TriviumBox::changed_message_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        
        if (!logger.has_errors)
            encrypt();

        m_ignoreTextInput = false;
    }
}

void TriviumBox::changed_encrypted_text()
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;
        
        if (!logger.has_errors)
            decrypt();

        m_ignoreTextInput = false;
    }
}

void TriviumBox::changed_ivector_text()
{
    changed_keyword_text();
}

void TriviumBox::changed_keyword_text()
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

Glib::ustring TriviumBox::crypt(const Glib::ustring& buffer)
{
    auto key_string = m_refKeywordBuffer->get_text();
    auto ivector = m_refInitializationVectorBuffer->get_text();
    auto base64_decoded = Glib::Base64::decode(buffer);
    auto num_bytes = base64_decoded.size();

    trivium_crypt(
         (u8*)key_string.data(), 
         (u8*)ivector.data(), 
         (u8*)base64_decoded.data(), 
         num_bytes);

    return Glib::Base64::encode(base64_decoded);
}

void TriviumBox::encrypt()
{
    auto buffer = m_refPlainTextBuffer->get_text();
    m_refEncryptedTextBuffer->set_text(crypt(buffer));
}


void TriviumBox::decrypt()
{
    auto buffer = m_refEncryptedTextBuffer->get_text();
    m_refPlainTextBuffer->set_text(crypt(buffer));
}


void TriviumBox::validate()
{
    logger_clear(&logger);

    int num_ivector_bytes = m_refInitializationVectorBuffer->get_bytes();

    if (num_ivector_bytes < 10)
    {
        logger_format_error(&logger, 
            "Not enough bytes in the initialization vector (%i). Must be at least 10.\n", num_ivector_bytes);
    }

    int num_keyword_bytes = m_refKeywordBuffer->get_bytes();

    if (num_keyword_bytes < 10)
    {
        logger_format_error(&logger, 
            "Not enough bytes in the keyword (%i). Must be at least 10.\n", 
            num_keyword_bytes);
    }
}

// Salsa20Box::~Salsa20Box() {}
