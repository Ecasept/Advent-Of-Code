use std::io::BufRead;

use crate::utils;
use aoc_macros::aoc;

type Button = Vec<usize>;

fn buttons_from_string(schematics: &[&str], pref: &str, suf: &str) -> Result<Vec<Button>, String> {
    schematics
        .iter()
        .map(|pattern| {
            (**pattern)
                .strip_prefix(pref)
                .expect("Invalid input")
                .strip_suffix(suf)
                .expect("Invalid input")
                .split(",")
                .map(|str| str.parse::<usize>().map_err(|e| e.to_string()))
                .collect::<Result<Button, String>>()
        })
        .collect()
}

fn generate_k_subsets<T: Clone>(
    vec: &Vec<T>,
    start: usize,
    left: usize,
    current: &mut Vec<T>,
    result: &mut Vec<Vec<T>>,
) {
    if left == 0 {
        result.push(current.clone());
        return;
    }

    for i in start..(vec.len() - left + 1) {
        current.push(vec[i].clone());
        generate_k_subsets(vec, i + 1, left - 1, current, result);
        current.pop();
    }
}

fn k_subsets_of<T: Clone>(vec: &Vec<T>, k: usize) -> Vec<Vec<T>> {
    let mut result = vec![];
    generate_k_subsets(vec, 0, k, &mut vec![], &mut result);
    return result;
}

fn does_solve(final_state: &str, pattern: &Vec<Button>) -> bool {
    // println!("Testing pattern {:?}", pattern);
    let mut state: Vec<bool> = (0..(final_state.len())).map(|_| false).collect();
    for p in pattern {
        for i in p {
            state[*i] = !state[*i];
        }
    }

    // println!(" Final state: {:?}", state);

    final_state
        .chars()
        .enumerate()
        .all(|(i, chr)| (chr == '#') == state[i])
}

fn num_for(state: &str, buttons: &Vec<Button>) -> u64 {
    // println!("Search for solution for {state} {:?}", buttons);
    for k in 1..buttons.len() {
        for pattern in k_subsets_of(buttons, k).iter() {
            if does_solve(state, pattern) {
                // println!("Found solution {k} with pattern {:?}", pattern);
                return k as u64;
            }
        }
    }
    panic!("No solution");
}

#[aoc(10, 1)]
fn p1() -> Result<u64, String> {
    let input = utils::get_i(10)?;
    let input: Vec<String> = input
        .lines()
        .map(|l| l.map_err(|e| e.to_string()))
        .collect::<Result<_, String>>()?;

    let inp: Vec<(&str, Vec<Button>, Button)> = input
        .iter()
        .map(|line| {
            let split: Vec<&str> = line.split(" ").collect();
            let first = split
                .first()
                .expect("Empty")
                .strip_prefix("[")
                .unwrap()
                .strip_suffix("]")
                .unwrap();
            let mut last = buttons_from_string(&[split.last().expect("Empty")], "{", "}")?;
            let between = &split[1..(split.len() - 1)];
            let schematics = buttons_from_string(between, "(", ")")?;

            Ok((first, schematics, last.remove(0)))
        })
        .collect::<Result<_, String>>()?;

    Ok(inp
        .iter()
        .fold(0, |acc, (state, schem, _)| acc + num_for(state, schem)))
}
