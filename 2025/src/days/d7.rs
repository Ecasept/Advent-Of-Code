use aoc_macros::aoc;
use std::collections::HashSet;

use crate::utils;
use crate::utils::grid::{Grid, ToCoords};

fn id<T>(arg: T) -> T {
    arg
}

trait D7Grid {
    fn is_splitter(&self, coords: impl ToCoords) -> bool;
}

impl D7Grid for Grid<char> {
    fn is_splitter(&self, coords: impl ToCoords) -> bool {
        *self.at(coords) == '^'
    }
}

#[aoc(7, 1)]
fn p1() -> Result<u64, String> {
    let mut heads: HashSet<(usize, usize)> = HashSet::new();
    let mut next_heads: HashSet<(usize, usize)> = HashSet::new();
    let grid = Grid::from_buf_read(utils::get_i(7)?, id);

    // Add starting point
    let start = (
        grid.rows()
            .first()
            .expect("Invalid input")
            .iter()
            .enumerate()
            .find(|(_, chr)| **chr == 'S')
            .expect("No starting point")
            .0,
        0,
    );
    heads.insert(start);

    let mut result = 0;

    let mut i = 0;
    loop {
        if i >= grid.height {
            break;
        }
        heads.iter().for_each(|tile| {
            if grid.is_splitter(*tile) {
                result += 1;
                next_heads.insert((tile.0 + 1, tile.1 + 1));
                next_heads.insert((tile.0 - 1, tile.1 + 1));
            } else {
                next_heads.insert((tile.0, tile.1 + 1));
            }
        });
        std::mem::swap(&mut heads, &mut next_heads);
        next_heads.clear();
        i += 1;
    }

    Ok(result as u64)
}

const INPUT_WIDTH: usize = 200;

struct HashSetHead {
    data: [usize; INPUT_WIDTH],
}

struct HashSetHeadIterator<'a> {
    set: &'a HashSetHead,
    i: usize,
}

impl Iterator for HashSetHeadIterator<'_> {
    type Item = (usize, usize);

    fn next(&mut self) -> Option<Self::Item> {
        loop {
            if self.i >= INPUT_WIDTH {
                return None;
            }
            let val = self.set.get(self.i);
            self.i += 1;
            if val != 0 {
                return Some((self.i - 1, val));
            } else {
                continue;
            }
        }
    }
}

impl HashSetHead {
    fn get(&self, pos: usize) -> usize {
        self.data[pos]
    }
    fn update(&mut self, pos: usize, func: impl Fn(usize) -> usize) {
        self.set(pos, func(self.data[pos]));
    }
    fn set(&mut self, pos: usize, val: usize) {
        self.data[pos] = val;
    }
    fn new() -> HashSetHead {
        HashSetHead {
            data: [0; INPUT_WIDTH],
        }
    }
    fn iter(&self) -> HashSetHeadIterator<'_> {
        HashSetHeadIterator { set: &self, i: 0 }
    }
    fn clear(&mut self) {
        for el in self.data.iter_mut() {
            *el = 0;
        }
    }
}

#[aoc(7, 2)]
fn p2() -> Result<u64, String> {
    let mut heads: HashSetHead = HashSetHead::new();
    let mut next_heads: HashSetHead = HashSetHead::new();
    let grid = Grid::from_buf_read(utils::get_i(7)?, id);

    // Add starting point
    let start_col = grid
        .rows()
        .first()
        .expect("Invalid input")
        .iter()
        .enumerate()
        .find(|(_, chr)| **chr == 'S')
        .expect("No starting point")
        .0;
    heads.set(start_col, 1);

    for i in 0..(grid.height) {
        heads
            .iter()
            .filter(|(col, _)| *col < grid.width)
            .for_each(|(col, val)| {
                if grid.is_splitter((col, i)) {
                    for new_col in [col + 1, col - 1] {
                        next_heads.update(new_col, |v| v + val);
                    }
                } else {
                    next_heads.update(col, |v| v + val);
                }
            });
        std::mem::swap(&mut heads, &mut next_heads);
        next_heads.clear();
    }

    Ok(heads.iter().map(|head| head.1 as u64).sum())
}
