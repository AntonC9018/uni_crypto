#include "shift.h"

ShiftBox::ShiftBox() 
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
{
    pack_start(m_KeyGrid);

    m_KeyGrid.set_halign(Gtk::ALIGN_CENTER);

    m_KeyGrid.attach(m_KeyHeader,    1, 1, 2);

    m_KeyGrid.attach(m_WidthLabel,   1, 2);
    m_KeyGrid.attach(m_WidthButton,  2, 2);

    m_KeyGrid.attach(m_HeightLabel,  1, 3);
    m_KeyGrid.attach(m_HeightButton, 2, 3);

    m_KeyGrid.attach(m_ColPermLable, 1, 4);
    m_KeyGrid.attach(m_ColPermEntry, 2, 4);

    m_KeyGrid.attach(m_RowPermLable, 1, 5);
    m_KeyGrid.attach(m_RowPermEntry, 2, 5);

    pack_start(m_KeySeparator);

    pack_start(m_UsageGrid);

    m_UsageGrid.set_halign(Gtk::ALIGN_CENTER);

    m_UsageGrid.attach(m_UsageHeader, 1, 1, 2);
    
    m_UsageGrid.attach(m_PlainTextLabel, 1, 2);
    m_UsageGrid.attach(m_CharsLeftLabel, 2, 2);
    m_UsageGrid.attach(m_PlainTextView,  1, 3, 2);

    m_UsageGrid.attach(m_EncryptedTextLabel, 1, 4, 1);
    m_UsageGrid.attach(m_EncryptedTextView,  1, 5, 2);

    pack_start(m_GridsBox, Gtk::PACK_EXPAND_PADDING);
    
    m_GridsBox.set_halign(Gtk::ALIGN_CENTER);

    m_GridsBox.pack_start(m_PlainGrid, Gtk::PACK_SHRINK);
    m_GridsBox.pack_start(m_EncryptedGrid, Gtk::PACK_SHRINK);


    m_KeyHeader.set_markup("<u><b>Key</b></u>");
    m_KeyHeader.set_margin_top(10);
    m_KeyHeader.set_margin_bottom(10);

    m_WidthLabel.set_text("Width");
    m_WidthAdjustment = Gtk::Adjustment::create(5, 1, 9, 1, 1, 0);
    m_WidthButton.set_adjustment(m_WidthAdjustment);
    m_WidthButton.set_digits(0);

    m_HeightLabel.set_text("Height");
    m_HeightAdjustment = Gtk::Adjustment::create(6, 1, 9, 1, 1, 0);
    m_HeightButton.set_adjustment(m_HeightAdjustment);
    m_HeightButton.set_digits(0);

    auto perm_entry_functor = sigc::mem_fun(*this, ShiftBox::changed_perm_entry);

    m_RowPermLable.set_text("Row Permutation");
    m_RowPermEntry.set_text("624153");
    m_RowPermEntry.signal_changed().connect(
        sigc::bind(perm_entry_functor, &m_RowPermEntry, m_HeightAdjustment.get()));

    m_ColPermLable.set_text("Col Permutation");
    m_ColPermEntry.set_text("31425");
    m_ColPermEntry.signal_changed().connect(
        sigc::bind(perm_entry_functor, &m_ColPermEntry, m_WidthAdjustment.get()));


    m_UsageHeader.set_markup("<u><b>Usage</b></u>");
    m_UsageHeader.set_margin_top(10);
    m_UsageHeader.set_margin_bottom(10);
    m_UsageHeader.set_size_request(400);

    auto text_changed_functor = sigc::mem_fun(*this, ShiftBox::changed_text); 

    m_PlainTextLabel.set_text("Plain Message");
    m_refPlainTextBuffer = Gtk::TextBuffer::create();
    m_refPlainTextBuffer->set_text("NuLasaPeMaineCePotiFaceAzi");
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

    m_PlainGrid.set_row_homogeneous(true);
    m_PlainGrid.set_column_homogeneous(true);
    m_PlainGrid.set_margin_end(20);

    m_EncryptedGrid.set_column_homogeneous(true);
    m_EncryptedGrid.set_column_homogeneous(true);
    m_EncryptedGrid.set_margin_end(20);
    
    logger_attach(&logger, this);

    show_all_children();

    m_ignoreTextInput = true;
    changed_perm_entry(&m_ColPermEntry, m_WidthAdjustment.get());
    m_ignoreTextInput = false;
    changed_perm_entry(&m_RowPermEntry, m_HeightAdjustment.get());
}


bool is_nonzero_digit(char ch)
{
    return ch >= '1' && ch <= '9';
}

void ShiftBox::changed_perm_entry(Gtk::Entry* entry, Gtk::Adjustment* adj)
{
    auto perm_text = entry->get_text();
    auto max_len = (size_t)(adj->get_value());
    auto sb = strb_make(max_len);
    bool mem[9] = {0};  // used digits, from 1 through nine
    
    auto& key_perm = (entry == &m_ColPermEntry) ? m_key.col_perm : m_key.row_perm;
    key_perm.clear();

    for (size_t i = 0; i < perm_text.size(); i++)
    {
        // 1 gets converted into 0
        char number = perm_text[i] - '1';
        if (number >= 0 && number < max_len)
        {
            if (!mem[number])
            {
                strb_chr(sb, perm_text[i]);
                key_perm.push_back(number);
            }
            mem[number] = true;
        }
    }

    // TODO: maybe use local buffer for this, but for that I need to tweak the strlib
    //       to accept buffers + length as parameters.
    auto str = strb_build(sb);
    entry->set_text(str.chars);
    str_free(str);

    // Validate the key (the length should be equal to width/heigth)
    changed_text(m_refPlainTextBuffer.get());

    // TODO: display errors.
}

void ShiftBox::changed_text(Gtk::TextBuffer* buffer)
{
    if (!m_ignoreTextInput)
    {
        m_ignoreTextInput = true;

        validate();
        if (!logger.has_errors)
        {
            do_crypto(buffer);
        }

        m_ignoreTextInput = false;
    }
}

void ShiftBox::recreate_grids()
{
    size_t width =  m_key.col_perm.size();
    size_t height = m_key.row_perm.size();

    // Fill in the PLAIN GRID with the message text and row-column labels
    {
        grid_remove_children(m_PlainGrid);

        // The indices for the plain message are permuted according to the key
        for (char col = 0; col < width; col++)
        {
            attach_bold_label(m_key.col_perm[col] + '1', col + 1, 0, m_PlainGrid); 
        }
        for (char row = 0; row < height; row++)
        {
            attach_bold_label(m_key.row_perm[row] + '1', 0, row + 1, m_PlainGrid);
        }

        // Message text
        size_t index = 0;
        auto message_text = m_refPlainTextBuffer->get_text();

        // First go through each column, for each of the rows. 
        // On the other hand, the encrypted text is the other way.
        for (size_t row = 0; row < height; row++)
        {
            for (size_t col = 0; col < width; col++, index++)
            {
                attach_label(
                    // When we leave the message text, insert spaces
                    // Could also just break out of the loop, since the layout is a grid layout
                    // and it doesn't matter if some of the spaces end up without objects
                    index < message_text.size() ? message_text[index] : ' ', 
                    col + 1, row + 1, m_PlainGrid); 
            }
        }
    }

    // Fill in the ENCRYPTED GRID with the ecrypted text and row-column labels
    {
        grid_remove_children(m_EncryptedGrid);

        // The column and row labels are in order, not permuted
        for (char col = 0; col < width; col++)
        {
            attach_bold_label(col + '1', col + 1, 0, m_EncryptedGrid); 
        }
        for (char row = 0; row < height; row++)
        {
            attach_bold_label(row + '1', 0, row + 1, m_EncryptedGrid);
        }

        // Encrypted text
        size_t index = 0;
        auto str = m_refEncryptedTextBuffer->get_text();

        // First go through each row, for each of the columns
        for (size_t col = 0; col < width; col++)
        {
            for (size_t row = 0; row < height; row++, index++)
            {
                attach_label(
                    index < str.size() ? str[index] : ' ', 
                    col + 1, row + 1, m_EncryptedGrid); 
            }
        }
    }
}

void ShiftBox::do_crypto(Gtk::TextBuffer* text_buffer)
{
    auto gtk_message = text_buffer->get_text();
    size_t message_size = std::min((size_t)gtk_message.size(), Shift::max_message_size(m_key));
    str_t message = str_copy(gtk_message.data(), message_size);

    if (text_buffer == m_refPlainTextBuffer.get())
    {
        str_t encrypted = Shift::encrypt(str_view(message), m_key);
        m_refEncryptedTextBuffer->set_text(encrypted.chars);
        str_free(encrypted);
    }  
    else if (text_buffer == m_refEncryptedTextBuffer.get())
    {
        str_t decrypted = Shift::decrypt(str_view(message), m_key);
        m_refPlainTextBuffer->set_text(decrypted.chars);
        str_free(decrypted);
    }

    str_free(message);

    {
        char buffer[64];
        sprintf(buffer, "Chars left: %zu", Shift::max_message_size(m_key) - message_size);
        m_CharsLeftLabel.set_text(buffer);
    }
    recreate_grids();
}

void ShiftBox::validate()
{
    logger_clear(&logger);

    if (m_key.col_perm.size() < (size_t)m_WidthAdjustment->get_value())
    {
        logger_format_error(&logger,
            "Error: Not enough numbers in the col permutation vector. Expected %zu, got %zu.\n", 
            (size_t)m_WidthAdjustment->get_value(), m_key.col_perm.size());
    }
    if (m_key.row_perm.size() < (size_t)m_HeightAdjustment->get_value())
    {
        logger_format_error(&logger,
            "Error: Not enough numbers in the row permutation vector. Expected %zu, got %zu.\n", 
            (size_t)m_HeightAdjustment->get_value(), m_key.row_perm.size());
    }
}