use std::io::BufRead;

use crate::utils;
use aoc_macros::aoc;

fn transpose<T: Clone>(v: Vec<Vec<T>>) -> Vec<Vec<T>> {
    let rows = v.len();
    let cols = v[0].len();
    (0..cols)
        .map(|col| (0..rows).map(|row| v[row][col].clone()).collect())
        .collect()
}

#[aoc(6, 1)]
fn p1() -> Result<u64, String> {
    let input = utils::get_i(6)?;

    let result = transpose(
        input
            .lines()
            .map(|s| {
                s.expect("Invalid input")
                    .split_whitespace()
                    .map(String::from)
                    .collect::<Vec<String>>()
            })
            .collect(),
    )
    .iter()
    // .map(|v| {
    //     println!("{:?}", v);
    //     v
    // })
    .map(|list| {
        list.iter().rev().fold((0, 0), |(op, acc), v| {
            let num = v.parse::<u64>();
            match op {
                1 => (op, acc + num.expect("Invalid number in input")),
                2 => (op, acc * num.expect("Invalid number in input")),
                0 => match v.as_ref() {
                    "+" => (1, 0),
                    "*" => (2, 1),
                    _ => panic!("Invalid op {v}"),
                },
                _ => panic!("Invalid state"),
            }
        })
    })
    .map(|(_, v)| v)
    .sum();

    Ok(result)
}

trait OpTestable {
    fn is_operand(&self) -> bool;
}

impl OpTestable for char {
    fn is_operand(&self) -> bool {
        return *self == '*' || *self == '+';
    }
}

type ChunkIteratorFn<T> = fn(&<T as Iterator>::Item, &<T as Iterator>::Item) -> bool;

trait IntoChunkIterator<T: Iterator> {
    fn chunked(self, func: ChunkIteratorFn<T>) -> ChunkIterator<T>;
}

struct ChunkIterator<T: Iterator> {
    iterator: T,
    func: ChunkIteratorFn<T>,
    prev: Option<T::Item>,
    acc: Vec<T::Item>,
}

impl<T: Iterator> Iterator for ChunkIterator<T> {
    type Item = Vec<T::Item>;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(p) = self.prev.take() {
            let next = self.iterator.next();
            if let Some(n) = next {
                if (self.func)(&p, &n) {
                    // Shift and try getting more elements
                    self.acc.push(p);
                    self.prev = Some(n);
                    return self.next();
                } else {
                    // Shift and abort
                    self.acc.push(p);
                    self.prev = Some(n);
                    return Some(std::mem::take(&mut self.acc));
                }
            } else {
                // self.prev is the last element
                self.acc.push(p);
                self.prev = None;
                return Some(std::mem::take(&mut self.acc));
            }
        } else {
            // Start or end of the iterator
            self.prev = self.iterator.next();
            if self.prev.is_none() {
                // End of iterator
                return None;
            }
            return self.next();
        }
    }
}

impl<T> IntoChunkIterator<T> for T
where
    T: Iterator,
{
    fn chunked(self, func: ChunkIteratorFn<T>) -> ChunkIterator<T> {
        ChunkIterator {
            iterator: self,
            func: func,
            prev: None,
            acc: Vec::new(),
        }
    }
}

fn op_fn(op: char) -> fn(u64, u64) -> u64 {
    match op {
        '+' => |a, b| a + b,
        '*' => |a, b| a * b,
        _ => panic!("Invalid op {op}"),
    }
}

fn neutral_element(op: char) -> u64 {
    match op {
        '+' => 0,
        '*' => 1,
        _ => panic!("Invalid op {op}"),
    }
}

#[aoc(6, 2)]
fn p2() -> Result<u64, String> {
    let input: Vec<String> = utils::get_i(6)?
        .lines()
        .map(|x| x.expect("Malformed input"))
        .collect();

    let mut col_info = input
        .last()
        .ok_or("No input lines")?
        .chars()
        .chunked(|_, b| !b.is_operand())
        .map(|chunk| (chunk.first().expect("Empty chunk").clone(), chunk.len()))
        .collect::<Vec<(char, usize)>>();
    col_info.last_mut().expect("no input lines").1 += 2; // numbers may be longer than the final line

    let split: Vec<Vec<String>> = input
        .iter()
        .rev()
        .skip(1)
        .map(|str| {
            let mut v = vec![];
            let mut char_iter = str.chars();
            col_info
                .iter()
                .for_each(|col| v.push(char_iter.by_ref().take((*col).1).collect::<String>()));
            return v;
        })
        .collect();

    Ok(col_info
        .iter()
        .enumerate()
        .map(|(col, (op, len))| {
            (0..*len)
                .map(|sub_col| {
                    let s = split
                        .iter()
                        .rev()
                        .map(|row| {
                            // get number
                            row[col].chars().nth(sub_col).unwrap_or(' ')
                        })
                        .collect::<String>();

                    if s.trim().is_empty() {
                        neutral_element(*op)
                    } else {
                        s.trim().parse::<u64>().unwrap()
                    }
                })
                .fold(neutral_element(*op), op_fn(*op))
        })
        .sum())
}
