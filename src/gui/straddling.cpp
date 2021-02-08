#include "straddling.h"

StraddlingBox::StraddlingBox()
    : m_EncryptedDisplay_GroupsButton("Show encrypted message in groups of")
    , m_EncryptedDisplay_ConcatenateButton("Concatenate the message")
    , Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,       1, 1, 2);

    m_KeyGrid.attach(m_AlphabetLabel,   1, 2);
    m_KeyGrid.attach(m_AlphabetCombo,   2, 2);

    m_KeyGrid.attach(m_RowIndicesLabel, 1, 3);
    m_KeyGrid.attach(m_RowIndicesEntry, 2, 3);

    m_KeyGrid.attach(m_ScrambleLabel,   1, 4);
    m_KeyGrid.attach(m_ScrambleEntry,   2, 4);

    m_KeyGrid.attach(m_KeywordLabel,    1, 5);
    m_KeyGrid.attach(m_KeywordEntry,    2, 5);

    pack_start(m_UsageGrid);

    m_UsageGrid.set_halign(Gtk::ALIGN_CENTER);

    m_UsageGrid.attach(m_UsageHeader, 1, 1, 2);
    
    m_UsageGrid.attach(m_PlainTextLabel, 1, 2, 2);
    m_UsageGrid.attach(m_PlainTextView,  1, 3, 2);

    m_UsageGrid.attach(m_EncryptedTextLabel, 1, 4, 2);
    m_UsageGrid.attach(m_EncryptedTextView,  1, 5, 2);

    m_UsageGrid.attach(m_EncryptedDisplay_GroupsButton,      1, 6, 1);
    m_UsageGrid.attach(m_EncryptedDisplay_GroupLengthButton, 2, 6, 1);
    m_UsageGrid.attach(m_EncryptedDisplay_ConcatenateButton, 1, 7, 1);

    pack_start(m_TableGrid);

    m_TableGrid.set_halign(Gtk::ALIGN_CENTER);


    m_KeyHeader.set_markup("<u><b>Key</b></u>");
    m_KeyHeader.set_margin_top(10);
    m_KeyHeader.set_margin_bottom(10);

    m_AlphabetLabel.set_text("Alphabet");
    m_AlphabetCombo.append(latin.chars);
    m_AlphabetCombo.append(latin_numbers_underscore.chars);
    m_AlphabetCombo.set_active(1);
    m_AlphabetCombo.signal_changed().connect(
        sigc::mem_fun(*this, StraddlingBox::alphabet_changed)
    );

    m_RowIndicesLabel.set_text("Row Indices");
    m_RowIndicesEntry.set_text("138");
    m_RowIndicesEntry.signal_changed().connect(
        sigc::mem_fun(*this, StraddlingBox::row_indices_changed)
    );

    m_ScrambleLabel.set_text("Scramble Word");
    m_ScrambleEntry.set_text("PLAYWRIGHT");
    m_ScrambleEntry.signal_changed().connect(
        sigc::mem_fun(*this, StraddlingBox::scramble_changed)
    );

    m_KeywordLabel.set_text("Keyword");
    m_KeywordEntry.set_text("MURPHY_");
    m_KeywordEntry.signal_changed().connect(
        sigc::mem_fun(*this, StraddlingBox::keyword_changed)
    );

    m_UsageHeader.set_markup("<u><b>Usage</b></u>");
    m_UsageHeader.set_margin_top(10);
    m_UsageHeader.set_margin_bottom(10);
    m_UsageHeader.set_size_request(400);

    auto text_changed_functor = sigc::mem_fun(*this, StraddlingBox::do_crypto); 

    m_PlainTextLabel.set_text("Plain Message");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("CLOCKS_WILL_RUN_MORE_QUICKLY_DURING_FREE_TIME");
    m_refPlainTextBuffer->signal_changed().connect(
        sigc::bind(text_changed_functor, m_refPlainTextBuffer.get())
    );
    m_PlainTextView.set_buffer(m_refPlainTextBuffer);
    m_PlainTextView.set_wrap_mode(Gtk::WRAP_WORD);

    m_EncryptedTextLabel.set_text("Encrypted Message");
    m_refEncryptedTextBuffer = Gtk::TextBuffer::create();
    m_refEncryptedTextBuffer->signal_changed().connect(
        sigc::bind(text_changed_functor, m_refEncryptedTextBuffer.get())
    );
    m_EncryptedTextView.set_buffer(m_refEncryptedTextBuffer);
    m_EncryptedTextView.set_wrap_mode(Gtk::WRAP_WORD);

    m_refGroupLengthAdjustment = Gtk::Adjustment::create(5, 1, 10, 1, 1, 0);
    m_EncryptedDisplay_GroupLengthButton.set_adjustment(m_refGroupLengthAdjustment);

    m_EncryptedDisplay_GroupsButton.join_group(m_EncryptedDisplay_ConcatenateButton);

    m_TableGrid.set_row_homogeneous(true);
    m_TableGrid.set_column_homogeneous(true);
    m_TableGrid.set_margin_end(20);

    show_all_children();

    m_ignoreAnyInput = true;
    refresh_alphabet();
    puts("refresh_keyword");
    refresh_keyword();
    puts("refresh_scramble");
    refresh_scramble();
    puts("refresh_row_indices");
    refresh_row_indices();
    puts("refresh_charset");
    refresh_charset();
    puts("refresh_order");
    refresh_order();
    do_crypto(m_refPlainTextBuffer.get());
}

void StraddlingBox::alphabet_changed()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;
        refresh_alphabet();     // Save the new alphabet
        refresh_keyword();      // Remove characters not in alphabet from the keyword.
        refresh_scramble();     // Remove characters not in the alphabet from the scramble.
        refresh_order();        // Satisfy the transitive dependency scramble -> order.
        refresh_charset();      // Satisfy the transitive and direct dependency alphabet -> (keyword) -> charset
        do_crypto(m_refPlainTextBuffer.get());
    }
}

void StraddlingBox::keyword_changed()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;
        refresh_keyword();      // Remove characters not in alphabet from the keyword.
        refresh_charset();      // Satisfy the dependency keyword -> charset
        do_crypto(m_refPlainTextBuffer.get());
    }
}

void StraddlingBox::scramble_changed()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;
        refresh_scramble();
        refresh_order();
        do_crypto(m_refPlainTextBuffer.get());
    }
}

void StraddlingBox::row_indices_changed()
{
    if (!m_ignoreAnyInput)
    {
        m_ignoreAnyInput = true;
        refresh_row_indices();
        do_crypto(m_refPlainTextBuffer.get());
    }
}

void StraddlingBox::refresh_alphabet()
{
    // I hate the fact that the string is newly allocated and there is nothing
    // I can do about it. Why not just allow me to poke into the buffer itself, to prevent any allocations?
    // I just don't get it. This is the reason why we have 10 billion extra string allocations
    // that could've simply been prevented altogether.
    auto gtk_alphabet = m_AlphabetCombo.get_active_text();
    // The stinky conversion I have to do time after time here, since my functions
    // expect a null terminated string and the size. By the way, c_str() most likely
    // does an allocation and a copy too, eugh, it stinks.
    if (m_alphabet.chars != 0)
    {
        str_free(m_alphabet);
    }
    m_alphabet = str_copy(gtk_alphabet.data(), gtk_alphabet.size());
    // Since the alphabet is taken out of a selection, we know it is valid.
}

void StraddlingBox::refresh_keyword()
{
    leave_unique_in_alphabet(m_KeywordEntry, m_keyword);
}

void StraddlingBox::refresh_scramble()
{
    leave_unique_in_alphabet(m_ScrambleEntry, m_scramble);
}

void StraddlingBox::refresh_charset()
{
    if (m_char_set.chars != 0)
        str_free(m_char_set);

    m_char_set = alphabet_without_keyword(str_view(m_keyword), str_view(m_alphabet));
}

void StraddlingBox::refresh_order()
{
    m_order.clear();
    m_order = arrange(str_view(m_scramble), str_view(m_alphabet));
}

void StraddlingBox::refresh_row_indices()
{
    auto gtk_indices = m_RowIndicesEntry.get_text();
    auto sb = strb_make(gtk_indices.size());
    
    for (size_t i = 0; i < gtk_indices.size(); i++)
    {
        char ch = gtk_indices.at(i);
        if (is_digit(ch) && !strb_has_char(sb, ch))
        {
            strb_chr(sb, ch);
        }
    }

    auto new_text = strb_build(sb);
    m_RowIndicesEntry.set_text(new_text.chars);

    m_row_indices.clear();
    for (size_t i = 0; i < new_text.length; i++)
    {
        m_row_indices.push_back(new_text[i] - '0');
    }

    str_free(new_text);
}

void StraddlingBox::leave_unique_in_alphabet(Gtk::Entry& entry, str_t& current)
{
    auto t = entry.get_text();
    
    auto sb = strb_make(t.size());
    for (size_t i = 0; i < t.size(); i++)
    {
        char ch = t.at(i);
        // Leave just the unique characters that are in the alphabet
        if (!strb_has_char(sb, ch) && str_has_char(m_alphabet, ch))
        {
            strb_chr(sb, ch);
        }
    }
    str_t k = strb_build(sb);

    entry.set_text(k.chars);
    if (current.chars) { str_free(current); }
    current = k;
}

bool StraddlingBox::validate()
{
    bool valid = true;
    // Validate the length of row_indices.
    // 10 is the harcoded value for column count
    auto can_sustain = m_row_indices.size() * 10;
    // If the alphabet cannot be fully placed in those rows, taking up the entire space,
    // Then the keyword should be changed.
    auto keyword_size_increase_to_fit = m_char_set.length % 10;

    if (m_order.size() != 10)
    {
        printf("The size of the scramble must be 10 (currently %zu).\n", m_order.size());
        valid = false;
    }
    // if (m_keyword.length != m_order.size() - m_row_indices.size())
    // {
    //     printf("The size of the keyword must be %zu.\n", m_order.size() - m_row_indices.size());
    //     valid = false;
    // }
    if (keyword_size_increase_to_fit != 0)
    {
        s32 inv = std::abs(10 - (s32)keyword_size_increase_to_fit);
        if (inv < keyword_size_increase_to_fit)
        {
            printf("The keyword is too long. Consider removing %i characters.\n", inv);
        }
        else
        {
            printf("The alphabet cannot fully take up the space in the bottom of the table. "
                "Consider adding %zu unique characters into your keyword.\n", 
                keyword_size_increase_to_fit);
        }
        valid = false;
    }
    else if (can_sustain < m_char_set.length)
    {
        printf("The alphabet does not fit in %zu rows. Consider adding more row indices.\n", 
            m_row_indices.size());
        valid = false;
    }
    else if (can_sustain > m_char_set.length)
    {
        printf("Too many rows for the alphabet (%zu). Consider removing one or more row indices.\n", 
            m_row_indices.size());
        valid = false;
    }

    return valid;
}

void StraddlingBox::make_key()
{
    m_valid = validate();

    if (m_valid)
    {
        Straddling::destroy_key(m_key);
        m_key = Straddling::make_key(str_view(m_keyword), m_order, m_row_indices, str_view(m_char_set), 10);
        recreate_grid();
    }
    else
    {
        // TODO: show errors.
        // TODO: lock inputs and grey out the table.
    }
}

void StraddlingBox::do_crypto(Gtk::TextBuffer* text_buffer)
{
    make_key();
    if (m_valid) 
    {
        auto gtk_message = text_buffer->get_text();
        size_t message_size = (size_t)gtk_message.size();
        str_view_t message = { gtk_message.c_str(), message_size };

        if (text_buffer == m_refPlainTextBuffer.get())
        {
            // auto encrypted = Straddling::encrypt(message, m_key);
            // m_refEncryptedTextBuffer->set_text(encrypted);
            // str_free(encrypted);
        }  
        else if (text_buffer == m_refEncryptedTextBuffer.get())
        {
            // TODO:
            // str_t decrypted = Straddling::decrypt(message, m_key);
            // m_refPlainTextBuffer->set_text(decrypted.chars);
            // str_free(decrypted);
        }
    }
    m_ignoreAnyInput = false;
}

static void remove_widget(Gtk::Widget& widget) 
{ 
    widget.get_parent()->remove(widget); 
}

static void attach_label(char text, int col, int row, Gtk::Grid& parent)
{
    auto t = Gtk::make_managed<Gtk::Label>();
    parent.attach(*t, col + 1, row + 1, 1, 1);
    char b[2] = { text, 0 };
    t->set_text(b);
    t->show();
}

void StraddlingBox::recreate_grid()
{
    m_TableGrid.forall(sigc::ptr_fun(remove_widget));
    
    for (size_t i = 0; i < m_order.size(); i++)
    {
        attach_label((char)m_order[i] + '0', i + 1, 0, m_TableGrid);
        attach_label(
            in_map(m_key.decrypt_header, m_order[i]) ? m_key.decrypt_header.at(m_order[i]) : '-', 
            i + 1, 1, m_TableGrid
        );
    }

    for (size_t i = 0; i < m_row_indices.size(); i++)
    {
        attach_label((char)m_row_indices[i] + '0', 0, i + 2, m_TableGrid);
    }

    for (const auto& [ch, index] : m_key.encrypt_normal)
    {
        size_t row = find_index(m_row_indices, index.first);
        attach_label(ch, index.second + 1, row + 2, m_TableGrid);
    }
}
