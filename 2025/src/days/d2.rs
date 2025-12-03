use aoc_macros::aoc;

use crate::utils;
use std::io::BufRead;


fn is_invalid(n: &u64) -> bool {
    let digits = n.ilog10() + 1;
    if digits%2 == 1 {
        return false;
    }
    return n % (10_u64.pow(digits/2) + 1) == 0;
}

fn brute() -> Result<(i64, Vec<(u64, u64)>), String> {
    let mut bv: Vec<(u64, u64)> = vec![];
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
        let mut acc = 0;
        let mut c = 0;
        for x in min..=max {
            if is_invalid(&x) {
                sum += x;
                acc += x;
                c += 1;
            }
        }
        bv.push((c, acc));
    }
    return Ok((sum.try_into().unwrap(), bv));
}

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
    
    let mut i = 0;
    for v in vals {
        let mut min: u64 = v[0].parse().expect("number expected");
        let mut max: u64 = v[1].parse().expect("number expected");
        
        // println!("Looking at range from {min} to {max}");

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

        // println!("Modified to {min} and {max}");
        // println!("With digits from {start_digits} to {end_digits}");
        let mut v = 0;
        for length in (start_digits..=end_digits).step_by(2) {
            let mut start = 10_u64.pow((length/2 - 1).try_into().unwrap());
            let mut end = 10_u64.pow((length/2).try_into().unwrap()) - 1;

            // println!(" Looking at range from {start} to {end}:");

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
                // println!("{first_half} {last_half} {half} {min} {max}");
                if last_half < first_half {
                    end -= 1;
                }
            }
            // println!(" Modified to {start} and {end}");

            if end < start {
                continue;
            }

            let conv = 10_u64.pow((length/2).try_into().unwrap()) + 1;

            let dif = end - start + 1;
            sum += conv * dif * (start+end)/2;
            v = conv * dif * (start+end)/2;

            // println!("# Found {dif} invalid ids")
        }

        
        // if v != bv[i].1 {
        //     println!("Mismatch at {i}: real {} ({}) vs false {}", bv[i].1, bv[i].0, v);
        // }
        i+=1;
    }

    return Ok(sum.try_into().unwrap());
}

pub fn day2_part2() -> Result<i64, String> {
    let input = utils::get_i(2)?;
    return Ok(0);
}
