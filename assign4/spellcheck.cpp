#include "spellcheck.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>


template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

// Function to tokenize a string
Corpus tokenize(const std::string& source) {
    // Find all whitespace positions
    auto whitespace_iters = find_all(source.begin(), source.end(), ::isspace);
    
    // Create the token corpus
    Corpus tokens;
    
    // Transform iterator pairs into tokens
    std::transform(
        whitespace_iters.begin(), 
        whitespace_iters.end() - 1,
        whitespace_iters.begin() + 1,
        std::inserter(tokens, tokens.end()),
        [&source](auto it1, auto it2) {
            return Token(source, it1, it2);
        }
    );
    
    // Remove empty tokens
    std::erase_if(tokens, [](const Token& token) {
        return token.content.empty();
    });
    
    return tokens;
}

// Function to check spelling
std::set<Mispelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
    namespace rv = std::ranges::views;
    
    // Create a view that filters and transforms tokens
    auto view = source 
        // Filter tokens not in the dictionary
        | rv::filter([&dictionary](const Token& token) {
            return !dictionary.contains(token.content);
        })
        // Transform tokens into misspellings with suggestions
        | rv::transform([&dictionary](const Token& token) {
            // Create view of suggestions with Levenshtein distance of 1
            auto suggestions_view = dictionary 
                | rv::filter([&token](const std::string& word) {
                    return levenshtein(token.content, word) == 1;
                });
            
            // Convert the view to a set of suggestions
            std::set<std::string> suggestions(suggestions_view.begin(), suggestions_view.end());
            return Mispelling{token, suggestions};
        })
        // Filter out misspellings without suggestions
        | rv::filter([](const Mispelling& m) {
            return !m.suggestions.empty();
        });
    
    // Convert the final view to a set of misspellings
    return std::set<Mispelling>(view.begin(), view.end());
}


#include "utils.cpp"