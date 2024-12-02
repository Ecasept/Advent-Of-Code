source ./.env
curl -o "Day $1/input1.txt" -H "Cookie: session=$SESSION_ID" "https://adventofcode.com/2024/day/$1/input"
