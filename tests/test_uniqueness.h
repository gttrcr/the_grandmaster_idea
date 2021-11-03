#pragma once

void test_3_uniqueness()
{
    const std::string test_name = "test_uniqueness";
    std::cout << test_name << " start" << std::endl;
    std::vector<std::string> files = utils::get_files(".", std::regex("test_matches_[0-9]+_[0-9]+." + std::string(FILE_EXT)));

    //1. check duplicates inside file itself
    std::cout << "\tinside file itself start" << std::endl;
    std::vector<std::vector<std::string>> files_th(global_threads);
    for (unsigned int i = 0; i < files.size(); i++)
        files_th[i % global_threads].push_back(files[i]);

    unsigned int duplicated_strings = 0;
    unsigned int total_strings = 0;
    std::for_each(
        std::execution::par_unseq,
        files_th.begin(),
        files_th.end(),
        [&duplicated_strings, &total_strings](auto&& item)
        {
            for (unsigned int i = 0; i < item.size(); i++)
            {
                //START - this cannot be inside a separate function
                std::fstream in_file;
                size_t data_size;
                std::unique_ptr<char[]> data;
                utils::prepare_string_view(item[i], in_file, data_size, data);
                std::vector<std::string_view> strings;
                for (size_t i = 0, start = 0; i < data_size; ++i)
                    if (data[i] == '\n')
                    {
#ifdef __linux__
                        strings.emplace_back(data.get() + start, i - start);
#elif _WIN32
                        strings.emplace_back(data.get() + start, i - start - 1);
#endif
                        start = i + 1;
                    }
                in_file.close();
                //STOP - this cannot be inside a separate function

                remove(item[i].c_str());
                unsigned int strings_size_before = strings.size();
                total_strings += strings_size_before;
                utils::remove_duplicates(strings);
                duplicated_strings += (strings_size_before - strings.size());

                std::ofstream out_file(item[i]);
                std::ostream_iterator<std::string_view> out_file_it(out_file, "\n");
                std::copy(strings.begin(), strings.end(), out_file_it);
                out_file.close();
            }
        });
    std::for_each(files_th.begin(), files_th.end(), [](std::vector<std::string>& v) { v.clear(); });
    std::cout << "\ttotal strings " << total_strings << std::endl << "\tduplicated strings " << duplicated_strings << std::endl;
    std::cout << "\tinside file itself end" << std::endl;

    //2. check duplicates between files
    duplicated_strings = 0;
    total_strings = 0;
    std::cout << "\tbetween files start" << std::endl;
    for (unsigned int i = 0; i < files.size(); i++)
    {
        std::string ref_file = files[i];

        //Load content of ref_file
        //START - this cannot be inside a separate function
        std::fstream if_ref_file;
        size_t data_size;
        std::unique_ptr<char[]> data;
        utils::prepare_string_view(ref_file, if_ref_file, data_size, data);
        std::vector<std::string_view> ref_strings;
        for (size_t i = 0, start = 0; i < data_size; ++i)
            if (data[i] == '\n')
            {
#ifdef __linux__
                ref_strings.emplace_back(data.get() + start, i - start);
#elif _WIN32
                ref_strings.emplace_back(data.get() + start, i - start - 1);
#endif
                start = i + 1;
            }
        if_ref_file.close();
        //STOP - this cannot be inside a separate function

        std::for_each(files_th.begin(), files_th.end(), [](std::vector<std::string>& v) { v.clear(); });
        for (unsigned int f = i + 1; f < files.size(); f++)
            files_th[f % global_threads].push_back(files[f]);

        std::for_each(
            std::execution::par_unseq,
            files_th.begin(),
            files_th.end(),
            [&ref_strings, &duplicated_strings, &total_strings](auto&& item)
            {
                for (unsigned int v = 0; v < item.size(); v++)
                {
                    //Load v-th file from item
                    //START - this cannot be inside a separate function
                    std::fstream in_file;
                    size_t data_size;
                    std::unique_ptr<char[]> data;
                    utils::prepare_string_view(item[v], in_file, data_size, data);
                    std::vector<std::string_view> strings;
                    for (size_t i = 0, start = 0; i < data_size; ++i)
                        if (data[i] == '\n')
                        {
#ifdef __linux__
                            strings.emplace_back(data.get() + start, i - start);
#elif _WIN32
                            strings.emplace_back(data.get() + start, i - start - 1);
#endif
                            start = i + 1;
                        }
                    in_file.close();
                    //STOP - this cannot be inside a separate function

                    unsigned int strings_size_before = strings.size();
                    total_strings += strings_size_before;
                    //Compare ref_strings (from ref_file) with strings (from v-th file from item)
                    std::for_each(ref_strings.begin(), ref_strings.end(), [&strings](const std::string_view& sv_ref)
                        { strings.erase(std::remove_if(strings.begin(), strings.end(), [&sv_ref](const std::string_view& sv)
                            { return sv_ref == sv; }),
                            strings.end()); });
                    duplicated_strings += (strings_size_before - strings.size());

                    //remove the file and replace with unique strings values
                    remove(item[v].c_str());
                    std::ofstream out_file(item[v]);
                    std::ostream_iterator<std::string_view> out_file_it(out_file, "\n");
                    std::copy(strings.begin(), strings.end(), out_file_it);
                    out_file.close();
                }
            });
    }
    std::cout << "\ttotal strings " << total_strings << std::endl << "\tduplicated strings " << duplicated_strings << std::endl;
    std::cout << "\tbetween files end" << std::endl;

    std::cout << test_name << " end" << std::endl;
}