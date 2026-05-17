from cs50 import get_string
import re

num = get_string("Number: ")
num1 = int(num)
length = len(num)
sum = 0
for i in range(length):
    if i % 2 != 0:
        if (num1 % 10) * 2 >= 10:
            sum += (((num1 % 10) * 2) % 10) + (((num1 % 10) * 2) // 10)
        else:
            sum = sum + ((num1 % 10) * 2)
    else:
        sum = sum + (num1 % 10)
    # print(sum, num1)
    num1 = num1 // 10
if sum % 10 != 0:
    # print (sum)
    print("INVALID")
else:
    if re.match(r'^3[47]', num) and length == 15:
        print('AMEX')
    elif re.match(r'^5[1-5]', num) and length == 16:
        print('MASTERCARD')
    elif re.match(r'^4', num) and length in (13, 16):
        print('VISA')
    else:
        print('INVALID')
