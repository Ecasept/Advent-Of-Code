use aoc_macros::aoc;

use crate::utils;
use std::{cmp, collections::HashSet, io::BufRead};

#[aoc(2, 1)]
pub fn day2_part1() -> Result<i64, String> {
    //let (bvs, bv) = brute().unwrap();
    let input = utils::get_i(2)?;
    let line = match input.lines().next() {
        Some(l) => match l {
            Ok(l) => l,
            Err(e) => return Err(e.to_string()),
        },
        None => return Err("No line to process in file!".to_owned()),
    };

    let vals = line
        .split(",")
        .map(|range| range.split("-").collect::<Vec<&str>>());

    let mut sum = 0_u64;
    
    for v in vals {
        let mut min: u64 = v[0].parse().expect("number expected");
        let mut max: u64 = v[1].parse().expect("number expected");
        
        let mut start_digits  = v[0].len() as u64;
        let mut end_digits = v[1].len() as u64;

        if start_digits % 2 == 1 {
            // Move the first bigger number that has even digits
            min = 10_u64.pow((start_digits).try_into().unwrap());
            start_digits += 1;
        }
        if end_digits % 2 == 1 {
            // Move the first smaller number that has even digits
            max = 10_u64.pow((end_digits - 1).try_into().unwrap()) - 1;
            end_digits -= 1;
        }

        for length in (start_digits..=end_digits).step_by(2) {
            let mut start = 10_u64.pow((length/2 - 1).try_into().unwrap());
            let mut end = 10_u64.pow((length/2).try_into().unwrap()) - 1;

            let half = 10_u64.pow((length / 2).try_into().unwrap());
            if length == start_digits {
                let first_half = min / half;
                start = first_half;
                let last_half = min % half;
                // First can't be used as the last digits is to large, so skip it
                if last_half > first_half {
                    start += 1;
                }
            }
            if length == end_digits {
                let first_half = max / half;
                end = first_half;
                let last_half = max % half;
                if last_half < first_half {
                    end -= 1;
                }
            }

            if end < start {
                continue;
            }

            let conv = 10_u64.pow((length/2).try_into().unwrap()) + 1;

            let dif = end - start + 1;
            sum += conv * dif * (start+end)/2;
        }
    }

    return Ok(sum.try_into().unwrap());
}

fn ten(amount: u64) -> u64 {
    return 10_u64.pow((amount-1) as u32);
}

fn get_conv(len: u64, step: u64) -> u64 {
    let mut conv = 0_u64;
    for i in (0..len).step_by(step.try_into().unwrap()) {
        conv += ten(i+1);
    }
    return conv;
}

fn get_division(num: u64, len: u64, step: u64) -> Vec<u64> {
    let mut result = vec![];
    for x in (step..=len).step_by(step.try_into().unwrap()) {
        let conv = ten(x + 1);
        let conv_prev = ten(x+1-step);
        let num_with_behind = num % conv;
        let digits = num_with_behind / conv_prev;
        result.insert(0, digits);
    }
    return result;
}

fn get_min_num_above(division: &Vec<u64>, num: u64, conv: u64) -> u64{
    let candidate = division[0];
    if candidate * conv >= num {
        return candidate;
    } else {
        return candidate + 1;
    }
}

fn get_max_num_below(division: &Vec<u64>, num: u64, conv: u64) -> u64{
    let candidate = division[0];
    if candidate * conv <= num {
        return candidate;
    } else {
        return candidate - 1;
    }
}
#[aoc(2, 2)]
pub fn day2_part2() -> Result<i64, String> {
    let input = utils::get_i(2)?;
    let line = match input.lines().next() {
        Some(l) => match l {
            Ok(l) => l,
            Err(e) => return Err(e.to_string()),
        },
        None => return Err("No line to process in file!".to_owned()),
    };

    let vals = line
        .split(",")
        .map(|range| range.split("-").collect::<Vec<&str>>());

    let mut sum = 0_u64;
    
    for v in vals {
        // Each line
        let min: u64 = v[0].parse().expect("number expected");
        let max: u64 = v[1].parse().expect("number expected");
        
        let start_digits  = v[0].len() as u64;
        let end_digits = v[1].len() as u64;
        for length in start_digits..=end_digits {
            // Confine to numbers with only length digits
            let max_alt = ten(length + 1) - 1; // highest number with length digits
            let max = cmp::min(max_alt, max);
            let min_alt = ten(length); // lowest number with length digits
            let min = cmp::max(min_alt, min);

            let mut found = HashSet::new();

            for step in 1..length {
                if length % step != 0 {
                    // Skip non-factors
                    continue;
                }
                let conv = get_conv(length, step);
    
                let min_div = get_division(min, length, step);
                let max_div = get_division(max, length, step);
                let start = get_min_num_above(&min_div, min, conv);
                let end = get_max_num_below(&max_div, max, conv);

                if start > end {
                    continue;
                }

                for a in start..=end {
                    found.insert(a*conv);
                }
            }
            sum += found.iter().sum::<u64>();
        }
    }

    return Ok(sum.try_into().unwrap());
}
