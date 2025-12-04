use aoc_macros::aoc;

use crate::utils;
use std::io::BufRead;

#[aoc(1, 1)]
pub fn day1_part1() -> Result<u64, String> {
    let input = utils::get_input(1)?;
    let mut val = 50;
    let mut sum = 0;
    for line in input.lines() {
        let line = match line {
            Ok(l) => l,
            Err(e) => return Err(e.to_string()),
        };

        let dir = line.chars().nth(0).ok_or("Invalid input format")?;
        let amount = line
            .chars()
            .skip(1)
            .collect::<String>()
            .parse::<i64>()
            .map_err(|e| e.to_string())?;
        let sign = if dir == 'R' { 1 } else { -1 };
        val = (val + sign * amount).rem_euclid(100);
        if val == 0 {
            sum += 1;
        }
    }

    return Ok(sum);
}

#[aoc(1, 2)]
pub fn day1_part2() -> Result<u64, String> {
    let input = utils::get_i(1)?;
    let mut val = 50;
    let mut sum = 0_u64;
    for line in input.lines() {
        let line = match line {
            Ok(l) => l,
            Err(e) => return Err(e.to_string()),
        };

        let dir = line.chars().nth(0).ok_or("Invalid input format")?;
        let amount = line
            .chars()
            .skip(1)
            .collect::<String>()
            .parse::<i64>()
            .map_err(|e| e.to_string())?;
        let sign = if dir == 'R' { 1 } else { -1 };
        let new_val = val + sign * amount;

        if new_val >= 100 {
            sum += (new_val / 100) as u64
        } else if new_val <= 0 {
            sum += ((new_val.abs()) / 100 + 1) as u64;

            if val == 0 {
                sum -= 1;
            }
        }

        val = new_val.rem_euclid(100);
    }

    Ok(sum)
}
