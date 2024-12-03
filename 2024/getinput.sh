source ./.env
curl -o "Day $1/input.txt" -H "Cookie: session=$SESSION_ID" "https://adventofcode.com/2024/day/$1/input"
