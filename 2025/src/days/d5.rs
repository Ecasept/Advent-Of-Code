use std::{cmp, io::BufRead};

use aoc_macros::aoc;

use crate::utils;

#[derive(PartialEq, Eq)]
enum ParsingState {
    Ranges,
    Values
}

trait Range {
    fn is_inside(&self, num: u64) -> bool;
}
impl Range for (u64, u64) {
    fn is_inside(&self, num: u64) -> bool {
        num >= self.0 && num <= self.1
    }
}


// #[aoc(5, 1)]
#[allow(dead_code)]
fn p1() -> Result<u64, String> {
    let mut ranges = vec![];
    let mut sum = 0;
    let mut parsing_state = ParsingState::Ranges;
    for line in utils::get_i(5)?.lines() {
        let line = line.unwrap();
        if line == "" {
            parsing_state = ParsingState::Values;
            continue;
        }
        match parsing_state {
            ParsingState::Ranges => {
                let (x, y) = line.split_once("-").unwrap();
                let x = x.parse::<u64>().unwrap();
                let y = y.parse::<u64>().unwrap();
                ranges.push((x,y));
            }
            ParsingState::Values => {
                let num = line.parse::<u64>().unwrap();
                if ranges.iter().any(|r| r.is_inside(num)) {
                    sum += 1;
                }
            }
        }
    }

    Ok(sum)
}

fn binary_search(ranges: &Vec<(u64, u64)>, num: u64, min: usize, max: usize) -> bool {
    if min > max {
        return false;
    }
    let pivoti = (max+min)/2;
    let pivot = ranges[pivoti];
    if num >= pivot.0 {
        if num <= pivot.1 {
            return true;
        }
        return binary_search(ranges, num, pivoti + 1, max);
    }
    return binary_search(ranges, num, min, pivoti - 1);
}

fn merge_sorted_ranges(ranges: Vec<(u64, u64)>) -> Vec<(u64, u64)> {
    let mut new_ranges: Vec<(u64, u64)> = vec![];
    let mut last_end = 0_u64;
    for (start, end) in ranges.iter() {
        if *start <= last_end {
            if *end <= last_end {
                // skip
                continue;
            }
            // merge
            last_end = *end;
            new_ranges.last_mut().unwrap().1 = *end;
        } else {
            // add
            last_end = *end;
            new_ranges.push((*start, *end));
        }
    }
    return new_ranges;
}

#[aoc(5, 1)]
fn p1_better() -> Result<u64, String> {
    let mut ranges = vec![];
    let mut sum = 0;
    let mut parsing_state = ParsingState::Ranges;
    for line in utils::get_i(5)?.lines() {
        let line = line.unwrap();
        if line == "" {
            parsing_state = ParsingState::Values;
            ranges.sort();
            ranges = merge_sorted_ranges(ranges);
            continue;
        }
        match parsing_state {
            ParsingState::Ranges => {
                let (x, y) = line.split_once("-").unwrap();
                let x = x.parse::<u64>().unwrap();
                let y = y.parse::<u64>().unwrap();
                ranges.push((x,y));
            }
            ParsingState::Values => {
                let num = line.parse::<u64>().unwrap();
                if binary_search(&ranges, num, 0, ranges.len()-1) {
                    sum += 1;
                }
            }
        }
    }

    Ok(sum)
}

// #[aoc(5, 2)]
#[allow(dead_code)]
fn p2() -> Result<u64, String> {
    let mut ranges = vec![];
    let mut sum = 0;
    for line in utils::get_i(5)?.lines() {
        let line = line.unwrap();
        if line == "" {
            break;
        }
        let (x, y) = line.split_once("-").unwrap();
        let mut x = x.parse::<u64>().unwrap();
        let mut y = y.parse::<u64>().unwrap();

        let mut i = 0;
        loop {
            if i == ranges.len() {
                // Searched all ranges
                ranges.push((x,y));
                break
            }
            let range: (u64, u64) = ranges[i];
            let x_inside = range.is_inside(x);
            let y_inside = range.is_inside(y);
            if x_inside && y_inside {
                // This range is inside another range skip it
                break;
            } else if !x_inside && !y_inside {
                if (x, y).is_inside(range.0) && (x, y).is_inside(range.1) {
                    // `range` is inside of this range, we can remove it
                    ranges.remove(i);
                } else {
                    // Ranges share no overlap
                    i+=1;
                }
            } else if x_inside && !y_inside {
                // Extend current range
                ranges.remove(i);
                x = range.0;
            } else if !x_inside && y_inside {
                // Extend current range
                ranges.remove(i);
                y = range.1;
            }
        }
    }

    for range in ranges {
        sum += range.1 - range.0 + 1;
    }

    Ok(sum)
}

#[aoc(5, 2)]
fn p2_better() -> Result<u64, String> {
    let mut ranges = vec![];
    let mut sum = 0;
    for line in utils::get_i(5)?.lines() {
        let line = line.unwrap();
        if line == "" {
            break;
        }
        let (x, y) = line.split_once("-").unwrap();
        let x = x.parse::<i64>().unwrap();
        let y = y.parse::<i64>().unwrap();
        ranges.push((x, y))
    }
    ranges.sort();
    let mut last_end = 0;
    for (start, end) in ranges {
        let start = cmp::max(last_end + 1, start);
        sum += cmp::max(0, end-start+1);
        last_end = cmp::max(end, last_end);
    }

    Ok(sum as u64)
}