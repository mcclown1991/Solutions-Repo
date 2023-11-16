#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "Solution Tester/SolutionTester.h"
#include <string>
#include <io.h>
#include <stdio.h>
#include <unordered_set>
#include <map>

namespace crossword {
	class Solution {
	public:
        void SearchHorizontal(std::string_view row, size_t index, std::vector<std::string> const& source, size_t y) {
            if (index > 0) {
                // search for left
                while (row.at(index - 1) != 'x') { --index; }
                //index is head
            }
            //search forward for full index;
            size_t count{};
            Word word;
            word.info.x = index;
            word.info.y = y;

            std::unordered_set<size_t> intersects;
            while (row.at(++index) != 'x') { 
                ++count;
                if (source.size() > y) {
                    if (source.at(y + 1).at(index) == 'o') {
                        // search vertical
                        word.intersect.emplace_back(index);
                    }
                }
                else if (y != 0) {
                    if (source.at(y - 1).at(index) == 'o') {
                        word.intersect.emplace_back(index);
                    }
                }
            }
            if (!count)
                return;
            word.size = count;
            word.info.horizontal = true;
        }

        void Preprocess(std::vector<std::string> const& board) {
            size_t y{};
            for (auto const& row : board) {
                for (auto i = 0; i < row.length(); ++i) {
                    if(row.at(i) == 'o')
                        SearchHorizontal(row, i, board, y);
                }
                ++y;
            }
        }

		size_t execute(std::vector<std::string> const& board, std::vector<std::string> const& words) {
            for (auto const& word : words) {
                if (!dictionary.count(word.size()))
                    dictionary.emplace(std::pair(word.size(), std::vector<std::pair<bool, std::string>>()));
                dictionary.at(word.size()).emplace_back(std::pair(false, word));
            }
            Preprocess(board);
            return 0;
		}

    private:
        struct Word{
            struct source {
                size_t x, y;
                bool horizontal;
            };
            size_t size;
            std::string word;
            source info;
            std::vector<size_t> intersect;
            std::vector<Word> intersect_words;
            std::unordered_set<std::string> not_fit;
        };

        std::map<size_t, std::vector<std::pair<bool, std::string>>> dictionary;
	};

    class Driver : public TestDriverBase {
        std::vector<std::string> ReadBoard(std::ifstream& input) {
            std::vector<std::string> result;
            std::string stream;
            std::getline(input, stream);
            std::getline(input, stream);
            size_t y = std::stoi(stream);
            for (auto i = 0; i < y; ++i) {
                std::getline(input, stream);
                result.emplace_back(stream);
            }
            return result;
        }

        std::vector<std::string> ReadWords(std::ifstream& input) {
            std::vector<std::string> result;
            std::string stream;
            while (getline(input, stream)) {
                result.emplace_back(stream);
            }
            return result;
        }

        virtual bool RunDriver() override {
            Solution sol;
            std::ifstream ifs("crossword_1.txt", std::ifstream::in);
            auto board = ReadBoard(ifs);
            auto words = ReadWords(ifs);
            auto result = sol.execute(board, words);
            std::cout << result;
            return true;
        }
    };
}