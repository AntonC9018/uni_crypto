#include "bazeries.h"

BazeriesBox::BazeriesBox()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,    1, 1, 2);

    m_KeyGrid.attach(m_KeywordLabel, 1, 2);
    m_KeyGrid.attach(m_KeywordEntry, 2, 2);

    m_KeyGrid.attach(m_NumericKeywordLabel, 1, 3);
    m_KeyGrid.attach(m_NumericKeywordEntry, 2, 3);

    pack_start(m_UsageGrid);

    m_UsageGrid.set_halign(Gtk::ALIGN_CENTER);

    m_UsageGrid.attach(m_UsageHeader, 1, 1, 2);
    
    m_UsageGrid.attach(m_PlainTextLabel, 1, 2, 2);
    m_UsageGrid.attach(m_PlainTextView,  1, 3, 2);

    m_UsageGrid.attach(m_EncryptedTextLabel, 1, 4, 2);
    m_UsageGrid.attach(m_EncryptedTextView,  1, 5, 2);

    pack_start(m_TableBox);

    m_TableBox.set_halign(Gtk::ALIGN_CENTER);

    m_TableBox.pack_start(m_TransposedAlphabet_Grid, Gtk::PACK_EXPAND_PADDING);
    m_TransposedAlphabet_Grid.set_halign(Gtk::ALIGN_CENTER);
    m_TransposedAlphabet_Grid.set_margin_end(20);

    m_TableBox.pack_start(m_PolybiosTable_Grid, Gtk::PACK_EXPAND_PADDING);
    m_PolybiosTable_Grid.set_halign(Gtk::ALIGN_CENTER);
    m_PolybiosTable_Grid.set_margin_end(20);


    m_KeyHeader.set_markup("<u><b>Key</b></u>");
    m_KeyHeader.set_margin_top(10);
    m_KeyHeader.set_margin_bottom(10);

    m_KeywordLabel.set_text("Keyword");
    m_KeywordEntry.set_text("SEVENTHOUSANDTHREEHUNDREDANDFIFTYTWO");
    m_KeywordEntry.signal_changed().connect(
        sigc::mem_fun(*this, BazeriesBox::changed_keyword)
    );

    m_NumericKeywordLabel.set_text("Numeric keyword");
    m_NumericKeywordEntry.set_text("7352");
    m_NumericKeywordEntry.signal_changed().connect(
        sigc::mem_fun(*this, BazeriesBox::changed_numeric_keyword)
    );

    m_UsageHeader.set_markup("<u><b>Usage</b></u>");
    m_UsageHeader.set_margin_top(10);
    m_UsageHeader.set_margin_bottom(10);
    m_UsageHeader.set_size_request(400);

    m_PlainTextLabel.set_text("Plain Message");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("ACLEARCONSCIENCEISUSUALLYTHESIGNOFABADMEMORY");
    m_refPlainTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, BazeriesBox::changed_message_text)
    );
    m_PlainTextView.set_buffer(m_refPlainTextBuffer);
    m_PlainTextView.set_wrap_mode(Gtk::WRAP_WORD);

    m_EncryptedTextLabel.set_text("Encrypted Message");
    m_refEncryptedTextBuffer = Gtk::TextBuffer::create();
    m_refEncryptedTextBuffer->signal_changed().connect(
        sigc::mem_fun(*this, BazeriesBox::changed_encrypted_text)
    );
    m_EncryptedTextView.set_buffer(m_refEncryptedTextBuffer);
    m_EncryptedTextView.set_wrap_mode(Gtk::WRAP_WORD);

    show_all_children();
    refresh_keyword();
    refresh_numeric_keyword();
    m_key = Bazeries::make_key(str_view(m_keyword), m_numeric_keyword);
    reread_plain_and_encrypt();
    recreate_grid();

    m_valid = true;
    m_ignoreAnyInput = false;
}

void BazeriesBox::changed_keyword()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;

        refresh_keyword();
        remake_key_and_encrypt();

        m_ignoreAnyInput = false;
    }
}

void BazeriesBox::changed_numeric_keyword()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;

        refresh_numeric_keyword();
        remake_key_and_encrypt();

        m_ignoreAnyInput = false;
    }
}

void BazeriesBox::changed_message_text()
{
    if (!m_ignoreAnyInput && m_valid)
    {
        m_ignoreAnyInput = true;

        reread_plain_and_encrypt();

        m_ignoreAnyInput = false;
    }
}

void BazeriesBox::changed_encrypted_text()
{
    if (!m_ignoreAnyInput && m_valid)
    {
        m_ignoreAnyInput = true;

        clear_message(m_encrypted_message);
        m_encrypted_message = refresh_text(m_refEncryptedTextBuffer.get());
        do_decrypt();

        m_ignoreAnyInput = false;
    }
}

void BazeriesBox::refresh_keyword()
{
    reset_keyword_from_entry_latin_upper(&m_KeywordEntry, &m_keyword);
}

void BazeriesBox::refresh_numeric_keyword()
{
    m_numeric_keyword.clear();
    auto gtk_text = m_NumericKeywordEntry.get_text();
    auto sb = strb_make(gtk_text.size());

    for (size_t i = 0; i < gtk_text.size(); i++)
    {
        if (gtk_text[i] >= '1' && gtk_text[i] <= '9')
        {
            strb_chr(sb, gtk_text[i]);
            m_numeric_keyword.push_back(gtk_text[i] - '0');
        }
    }

    auto t = strb_build(sb);
    m_NumericKeywordEntry.set_text({ t.chars, t.length });
    str_free(t);
}

// Returns the new text
Message BazeriesBox::refresh_text(Gtk::TextBuffer* textbuffer)
{
    auto gtk_message = textbuffer->get_text();
    auto delimited_sb = strb_make(gtk_message.size() * 2);
    auto plain_sb     = strb_make(gtk_message.size());

    size_t current_count = 0;
    size_t numeric_keyword_index = 0;

    for (size_t i = 0; i < gtk_message.size(); i++)
    {
        char ch = gtk_message.at(i);
        if (ch >= 'a' && ch <= 'z')
        {
            ch = ch - 'a' + 'A';
        }
        if (ch >= 'A' && ch <= 'Z')
        {
            if (current_count == m_key.numeric_keyword[numeric_keyword_index])
            {
                strb_chr(delimited_sb, ' ');
                numeric_keyword_index++;
                if (numeric_keyword_index == m_key.numeric_keyword.size())
                {
                    numeric_keyword_index = 0;
                }
                current_count = 0;
            }

            ch = m_key.poly.mapf(ch);
            strb_chr(plain_sb, ch);
            strb_chr(delimited_sb, ch);

            current_count++;
        }
    }
    Message result;
    result.plain     = strb_build(plain_sb);
    result.delimited = strb_build(delimited_sb);

    textbuffer->set_text({ result.delimited.chars, result.delimited.length });

    return result;
}

void BazeriesBox::validate()
{
    m_valid = true;

    if (m_keyword.length == 0)
    {
        printf("The keyword must contain at least one character");
        m_valid = false;
    }
    if (m_numeric_keyword.size() == 0)
    {
        printf("The numeric keyword must contain at least one number");
        m_valid = false;
    }
}

void BazeriesBox::make_key()
{
    Bazeries::destroy_key(m_key);
    m_key = Bazeries::make_key(str_view(m_keyword), m_numeric_keyword);
}

void BazeriesBox::clear_message(Message message)
{
    if (message.plain.chars != 0)
    {
        str_free(message.plain);
        str_free(message.delimited);
    }
}

void BazeriesBox::reread_plain_and_encrypt()
{
    m_plain_message = refresh_text(m_refPlainTextBuffer.get());
    do_encrypt();
}

void BazeriesBox::remake_key_and_encrypt()
{
    validate();
    if (m_valid)
    {
        make_key();
        recreate_grid();
        reread_plain_and_encrypt();
    }
}

void BazeriesBox::do_encrypt()
{
    auto encrypted = Bazeries::encrypt(str_view(m_plain_message.plain), m_key);
    clear_message(m_encrypted_message);
    m_encrypted_message.plain = encrypted;
    m_encrypted_message.delimited = Bazeries::delimit(str_view(encrypted), m_key); 
    m_refEncryptedTextBuffer->set_text(
        { m_encrypted_message.delimited.chars, m_encrypted_message.delimited.length });
}

void BazeriesBox::do_decrypt()
{
    auto decrypted = Bazeries::decrypt(str_view(m_encrypted_message.plain), m_key);
    clear_message(m_plain_message);
    m_plain_message.plain = decrypted;
    m_plain_message.delimited = Bazeries::delimit(str_view(decrypted), m_key); 
    m_refPlainTextBuffer->set_text(
        { m_plain_message.delimited.chars, m_plain_message.delimited.length });
}

void BazeriesBox::recreate_grid()
{
    {
        grid_remove_children(m_PolybiosTable_Grid);
        
        size_t index = 0;
        for (size_t row = 0; row < m_key.poly.dim; row++)
        {
            for (size_t col = 0; col < m_key.poly.dim; col++, index++)
            {
                attach_label(m_key.poly.table[index], col, row, m_PolybiosTable_Grid);
            }
        }
    }
    {
        grid_remove_children(m_TransposedAlphabet_Grid);
        
        size_t index = 0;
        for (size_t row = 0; row < m_key.poly.dim; row++)
        {
            for (size_t col = 0; col < m_key.poly.dim; col++, index++)
            {
                attach_label(m_key.transposed_alphabet[index], col, row, m_TransposedAlphabet_Grid);
            }
        }
    }
}