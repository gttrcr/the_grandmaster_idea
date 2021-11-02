#pragma once

void test_3_uniqueness()
{
    const std::string test_name = "test_uniqueness";
    std::cout << test_name << " start" << std::endl;
    std::vector<std::string> files = utils::get_files(".", std::regex("test_matches_[0-9]+_[0-9]+" + std::string(FILE_EXT)));

    for (unsigned int f = 0; f < files.size(); f++)
    {
        std::string ref_file = files[f];
        std::vector<std::vector<std::string>> files_th(global_threads);
        for (unsigned int i = f; i < files.size(); i++)
            files_th[i % global_threads].push_back(files[i]);

        std::for_each(
            std::execution::par_unseq,
            files_th.begin(),
            files_th.end(),
            [ref_file](auto&& item)
            {
                //START - this cannot be put inside a separate function
                size_t data_size;
                std::unique_ptr<char[]> data;
                utils::prepare_string_view(ref_file, data_size, data);
                std::vector<std::string_view> strings;
                for (size_t i = 0, start = 0; i < data_size; ++i)
                    if (data[i] == '\n')
                    {
                        strings.emplace_back(data.get() + start, i - start - 1);
                        start = i + 1;
                    }
                //STOP - this cannot be put inside a separate function

                for (unsigned int i = 0; i < item.size(); i++)
                {
                    std::ifstream in_file(item[i]);
                    std::string line;
                    unsigned int line_index = 0;
                    while (std::getline(in_file, line))
                    {
                        line_index++;
                        unsigned int count = 0;
                        for (unsigned int v = 0; v < strings.size(); v++)
                        {
                            bool is_same = strings[v] == line;
                            count += is_same;

                            //1 line is ok (the line itself), more the 1 is not ok
                            if (count > 1 && is_same && line_index != v + 1)
                                std::cout << "Duplicate line between " << ref_file << " (line " << line_index << ") and " << item[i] << " (line " << v + 1 << ")" << std::endl;
                        }
                    }
                    in_file.close();
                }
            });
    }

    std::cout << test_name << " end" << std::endl;
}