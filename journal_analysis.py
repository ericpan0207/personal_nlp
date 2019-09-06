#!/usr/bin/env python3
from collections import Counter
import os
import re
import random
from string import punctuation
import sys
import time

start = time.time()
entries = []
wordcount = Counter()
string_file = []
file_dict = {}
r = re.compile(r'[\s]+'.format(re.escape(punctuation)))

for root, dirs, files in os.walk("/mnt/c/Eric/Journal"):
    for file in files:
        file_words = []
        if file.endswith(".txt"):
            if file[0].isdigit():
                entries.append(file)
            with open(os.path.join(root, file), "r", encoding='UTF-8') as f:
                # Store an entire file into a string
                string_file = f.read()
                file_dict[file] = string_file
            with open(os.path.join(root, file), "r", encoding='UTF-8') as f:
                # Get all the words in a file
                file_words = f.read().split()
                # find file that specific words appear in
                # if '' in file_words:
                #    print(file)
        # Regex to split on punctation
        split_words = [r.split(s) for s in file_words]
        # Flatten list
        flat_words = [item for subitem in split_words for item in subitem if item]
        wordcount += Counter(flat_words)

print('Took: ', time.time() - start, ' seconds')

# Get a random journal entry
length_of_passage = 500
passage_date, random_passage = random.sample(file_dict.items(), 1)[0]
randomNum = random.randint(0, len(random_passage) - 100 + 1)
print('Date: ', passage_date)
print(random_passage[randomNum : randomNum + length_of_passage])

print('Number of entries: ', len(entries))
print('Total words: ', sum(wordcount.values()))
print('Unique words: ', len(list(wordcount)))

most_comm = wordcount.most_common(500)
# Find most common words that are capitalized (going for names here).
print([word for word in most_comm if len(word[0]) > 0 and word[0][0].isupper()])

# Find specific words
if len(sys.argv) > 1:
    word = sys.argv[1]
    if word in wordcount.keys():
        occurence = wordcount[word]
        if word.lower() in wordcount.keys():
            occurence += wordcount[word.lower()]
        print('The word: [', word, '] appeared: ', occurence, ' times')
