#pragma once

#include "result.h"

class file_layer_impl;
class file_layer_listener;

/**
 * This class represents the interface of the file layer.
 */
class file_layer
{
public:
    /**
     * Construct an instance of the file layer.
     */
    file_layer();

    /**
     * Destruct an instance of the file layer.
     */
    ~file_layer();

    /**
     * Open a file.
     * @param file_name The name of the file.
     * @return result_t::success if the file is sucessfully opened.
     */
    result_t open(const char* file_name);

    /**
     * Read a page from the file.
     * @param page_number The page to be read.
     * @param buffer The buffer to store the page.
     * @return result_t::success if the page is successfully read.
     */
    result_t read_page(int page_number, void* buffer);

    /**
     * Write a page to the file.
     * @param page_number The page to be written.
     * @param buffer The buffer to store the page.
     * @return result_t::success if the page is successfully written.
     */
    result_t write_page(int page_number, void* buffer);

    /**
     * Append a new blank page to the end of the file.
     * @param new_page_number [out] The new page number
     * @return result_t::success if a new blank page is successfully appended at the end of the file.
     */
    result_t append_page(int* new_page_number);

    /**
     * Specify the file layer listener for receiving callback.
     * @param file_layer_listener The file layer listener for receiving callback.
     * @return result_t::success if the file layer listener for receiving callback is successfully specified.
     */
    result_t set_file_layer_listener(file_layer_listener* file_layer_listener);

    /**
     * Obtain the number of pages.
     * @param num_pages [out] The number of pages.
     * @return result_t::success if the number of pages is successfully obtained.
     */
    result_t get_num_pages(int* num_pages);

    /**
     * Close the file.
     * @return result_t::success if the file is successfully closed.
     */
    result_t close();
private:
    file_layer_impl* m_impl;
};

/**
 * This class represents a collection of callback to be invoked by the file layer.
 */
class file_layer_listener
{
public:
    /**
     * Represents a callback to be called after the file is created.
     */
    virtual result_t on_after_file_created() = 0;

    /**
     * Represents a callback to be called after the file is loaded.
     */
    virtual result_t on_after_file_loaded() = 0;

    /**
     * Represents a callback to be called before the file is closed.
     */
    virtual result_t on_before_file_closed() = 0;
};
