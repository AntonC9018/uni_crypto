#include "vigenere.h"

VigenereBox::VigenereBox()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,    1, 1, 2);

    m_KeyGrid.attach(m_KeywordLabel, 1, 2);
    m_KeyGrid.attach(m_KeywordEntry, 2, 2);

    pack_start(m_UsageGrid);

    m_UsageGrid.set_halign(Gtk::ALIGN_CENTER);

    m_UsageGrid.attach(m_UsageHeader, 1, 1, 2);
    
    m_UsageGrid.attach(m_PlainTextLabel, 1, 2, 2);
    m_UsageGrid.attach(m_PlainTextView,  1, 3, 2);

    m_UsageGrid.attach(m_EncryptedTextLabel, 1, 4, 2);
    m_UsageGrid.attach(m_EncryptedTextView,  1, 5, 2);

    pack_start(m_KeyTable_Grid);
    m_KeyTable_Grid.set_halign(Gtk::ALIGN_CENTER);

    pack_start(m_EncryptionTable_Box);
    m_EncryptionTable_Box.set_halign(Gtk::ALIGN_CENTER);
    m_EncryptionTable_Box.pack_start(m_EncryptionTableLabel_Grid, Gtk::PACK_SHRINK);
    m_EncryptionTable_Box.pack_start(m_EncryptionTable_Grid, Gtk::PACK_SHRINK);


    m_KeyHeader.set_markup("<u><b>Key</b></u>");
    m_KeyHeader.set_margin_top(10);
    m_KeyHeader.set_margin_bottom(10);

    m_KeywordLabel.set_text("Keyword ");
    
    m_KeywordEntry.set_text("battista");
    m_KeywordEntry.signal_changed().connect(
        sigc::mem_fun(*this, VigenereBox::changed_keyword)
    );

    m_UsageHeader.set_markup("<u><b>Usage</b></u>");
    m_UsageHeader.set_margin_top(10);
    m_UsageHeader.set_margin_bottom(10);
    m_UsageHeader.set_size_request(400);

    m_PlainTextLabel.set_text("Plain Message");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("asimpleexample");
    m_refPlainTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, VigenereBox::changed_message_text)
    );
    m_PlainTextView.set_buffer(m_refPlainTextBuffer);
    m_PlainTextView.set_wrap_mode(Gtk::WRAP_WORD);

    m_EncryptedTextLabel.set_text("Encrypted Message");
    m_refEncryptedTextBuffer = Gtk::TextBuffer::create();
    m_refEncryptedTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, VigenereBox::changed_encrypted_text)
    );
    m_EncryptedTextView.set_buffer(m_refEncryptedTextBuffer);
    m_EncryptedTextView.set_wrap_mode(Gtk::WRAP_WORD);

    m_KeyTable_Grid.set_row_homogeneous(true);
    m_KeyTable_Grid.set_column_homogeneous(true);
    m_KeyTable_Grid.set_margin_end(20);
    
    attach_label(str_lit("Message:"),   0, 0, m_EncryptionTableLabel_Grid);
    attach_label(str_lit("Keyword:"),   0, 1, m_EncryptionTableLabel_Grid);
    attach_label(str_lit("Encrypted:"), 0, 2, m_EncryptionTableLabel_Grid);

    m_EncryptionTableLabel_Grid.set_row_homogeneous(true);
    m_EncryptionTableLabel_Grid.set_column_homogeneous(true);
    m_EncryptionTableLabel_Grid.set_margin_end(20);

    m_EncryptionTable_Grid.set_row_homogeneous(true);
    m_EncryptionTable_Grid.set_column_homogeneous(true);
    m_EncryptionTable_Grid.set_margin_end(20);

    show_all_children();

    changed_keyword();
}

void VigenereBox::changed_keyword()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;

        {
            auto keyword_text = m_KeywordEntry.get_text();
        
            auto sb = strb_make(keyword_text.size());
            for (size_t i = 0; i < keyword_text.size(); i++)
            {
                if (keyword_text[i] >= 'A' && keyword_text[i] <= 'Z')
                {
                    // Lower case all letters
                    strb_chr(sb, keyword_text[i] - 'A' + 'a');
                }
                else if (keyword_text[i] >= 'a' && keyword_text[i] <= 'z')
                {
                    strb_chr(sb, keyword_text[i]);
                }
            }
            str_t k = strb_build(sb);

            m_KeywordEntry.set_text(k.chars);
            if (m_keyword.chars) { str_free(m_keyword); }
            m_keyword = k;
        }

        recreate_key_grid();
        do_encrypt();
        recreate_encryption_grid();
    }
}

void VigenereBox::changed_message_text()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;
        recreate_key_grid();
        do_encrypt();
        recreate_encryption_grid();
    }
}

void VigenereBox::changed_encrypted_text()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;
        recreate_key_grid();
        do_decrypt();
        recreate_encryption_grid();
    }
}

void VigenereBox::do_encrypt()
{
    auto gtk_message = m_refPlainTextBuffer->get_text();
    str_view_t message = { gtk_message.c_str(), gtk_message.size() };
    auto encrypted = Vigenere::encrypt(message, { str_view(m_keyword) });
    m_refEncryptedTextBuffer->set_text(encrypted.chars);
    str_free(encrypted);
    m_ignoreAnyInput = false;
}

void VigenereBox::do_decrypt()
{
    auto gtk_message = m_refEncryptedTextBuffer->get_text();
    str_view_t message = { gtk_message.c_str(), gtk_message.size() };
    auto decrypted = Vigenere::decrypt(message, { str_view(m_keyword) });
    m_refPlainTextBuffer->set_text(decrypted.chars);
    str_free(decrypted);
    m_ignoreAnyInput = false;
}

void VigenereBox::recreate_encryption_grid()
{
    grid_remove_children(m_EncryptionTable_Grid);
    auto gtk_message   = m_refPlainTextBuffer->get_text();
    auto gtk_encrypted = m_refEncryptedTextBuffer->get_text();

    for (size_t i = 0; i < gtk_message.size(); i++)
    {
        char keyword_char = m_keyword[i % m_keyword.length];
        
        attach_label(gtk_message[i],   i + 1, 0, m_EncryptionTable_Grid);
        attach_label(keyword_char,     i + 1, 1, m_EncryptionTable_Grid);
        attach_label(gtk_encrypted[i], i + 1, 2, m_EncryptionTable_Grid);
    }
}

void VigenereBox::recreate_key_grid()
{
    grid_remove_children(m_KeyTable_Grid);

    for (size_t i = 0; i < m_keyword.length; i++)
    {
        attach_bold_label(m_keyword[i], 0, i, m_KeyTable_Grid);

        for (char j = 1; j < LATIN_LENGTH; j++)
        {
            attach_label(
                (m_keyword[i] + j - FIRST_CHARACTER) % LATIN_LENGTH + FIRST_CHARACTER,
                j, i, m_KeyTable_Grid
            ); 
        }
    }
}
