#!/bin/sh
read name
echo "$name It is a test"
read firstStr secondStr
echo "first string $firstStr, second string $secondSti"
read -p "please input a message:" -n 6 -t 5 -s password
echo -e "\npassword is $password"
