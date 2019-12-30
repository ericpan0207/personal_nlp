#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
namespace fs = std::experimental::filesystem;
using namespace std::chrono;

bool isTextFile(const std::string& s, const std::string& ending) {
    if (s.length() >= ending.length()) {
        return (0 == s.compare(s.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void traverse_files(
    const std::string& path,
    const std::string& file_extension,
    const std::regex& words_regex,
    std::unordered_map<std::string, int>* wordcount)
{
    std::string words;
    std::ifstream file_reader;
    int count = 0;
    int frequency = 100;

    // For each file / directory in path
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        std::string file = static_cast<std::string>(entry.path());
        // if the file is a txt file
        if (isTextFile(file, file_extension)) {
            file_reader.open(file);
            if (file_reader.is_open()) {
                while (getline(file_reader, words)) {
                    auto words_begin = std::sregex_iterator(
                        words.begin(),
                        words.end(),
                        words_regex);
                    auto words_end = std::sregex_iterator();
                    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
                    {
                        std::unordered_map<std::string, int>& counts = *wordcount;
                        ++counts[(*i).str()];
                    }
                }
                file_reader.close();
            }
            count++;
        }
    }

    std::cout << "Number of entries: " << count << std::endl;

    // Sorts most frequent words from unordered map to be displayed
    count = 0;
    std::map<int, std::string> frequentWords;
    for (const auto& n : *wordcount) {
        count += n.second;
        if (n.second > frequency && isupper(n.first[0])) {
            frequentWords[n.second] = n.first;
        }
    }
    for (auto n = frequentWords.rbegin(); n != frequentWords.rend(); ++n) {
        std::cout << n->second << " : " << n->first << ", ";
    }
    std::cout << std::endl;
    std::cout << "Total words: " << count << std::endl;
    std::cout << "Unique words: " << wordcount->size() << std::endl;
}

void find_specific_words(
    int argc,
    char** argv,
    std::unordered_map<std::string, int> wordcount)
{
    int occurrence = 0;
    std::string word = argv[1];
    if (wordcount.count(word) > 0) {
        occurrence = wordcount[word];
    }
    std::transform(word.begin(), word.end(), word.begin(),
        [](unsigned char c){ return std::tolower(c); });
    if (wordcount.count(word) > 0) {
        occurrence += wordcount[word];
    }
    std::cout << "The word: " << argv[1] << " appeared: "
        << occurrence << " times" << std::endl;
}

int main(int argc, char** argv) {
    auto start = high_resolution_clock::now();

    const std::string path = "/mnt/c/Eric/Journal";
    const std::string file_extension = ".txt";
    const std::regex words_regex("[^\\s.,;!?()]+");
    std::unordered_map<std::string, int> wordcount;

    traverse_files(path, file_extension, words_regex, &wordcount);
    if (argc > 1) {
        find_specific_words(argc, argv, wordcount);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count() / 1000000.0;
    std::cout << "Took: " << duration << " seconds" << std::endl;
    return 0;
}
