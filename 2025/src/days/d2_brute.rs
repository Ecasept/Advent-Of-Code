use crate::utils;
use std::io::BufRead;
use aoc_macros::aoc;


fn is_invalid(n: &u64) -> bool {
    let digits = n.ilog10() + 1;
    if digits%2 == 1 {
        return false;
    }
    return n % (10_u64.pow(digits/2) + 1) == 0;
}
fn is_invalid2(n: &u64) -> bool {
    let digits = n.ilog10() + 1;
    for len in 1..=(digits/2) {
        if digits % (len) != 0 {
            // skip non-factors
            continue;
        }

        let mut conv = 0;
        for j in ((0)..digits).step_by(len.try_into().unwrap()) {
            conv += 10_u64.pow(j)
        }
        if n % conv == 0 {
            return true;
        }
    }
    return false;
}

fn brute(p2: bool) -> Result<i64, String> {
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
        let min: u64 = v[0].parse().expect("number expected");
        let max: u64 = v[1].parse().expect("number expected");
        for x in min..=max {
            if if p2 {is_invalid2(&x)} else {is_invalid(&x)} {
                sum += x;
            }
        }
    }

    return Ok(sum.try_into().unwrap());
}


pub fn day2_part1() -> Result<i64, String> {
    return brute(false);
}

#[aoc(2, 2)]
pub fn day2_part2() -> Result<i64, String> {
    return brute(true);
}
