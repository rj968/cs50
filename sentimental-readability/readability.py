from cs50 import get_string

text = get_string("Text: ")
length = len(text)
words = 1
L = 0
s = 0
for i in range(length):
    if text[i].isalnum():
        L += 1
    if text[i] == " ":
        words += 1
    elif text[i] in ['.', '?', '!']:
        s += 1
L = (L / words) * 100
s = (s/words) * 100
readability = round(0.0588 * L - 0.296 * s - 15.8)
if readability < 1:
    print("Before Grade 1")
elif readability >= 16:
    print('Grade 16+')
else:
    print(f'Grade {readability}')


# Coleman-Liau index is computed as 0.0588 * L - 0.296 * S - 15.8,
# where L is the average number of letters per 100 words in the text,
# and S is the average number of sentences per 100 words in the text.
