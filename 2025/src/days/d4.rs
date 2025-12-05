use std::{collections::VecDeque, fs::File, io::{BufRead, BufReader}};
use crate::utils;

use aoc_macros::aoc;

fn in_range(a: isize, b: isize, width: usize, height: usize) -> bool {
    a >=0 && b >=0 && a < (width as isize) && b < (height as isize)
}

fn parse_data_to_vec(input: BufReader<File>) -> Result<Vec<Vec<char>>, String> {
    let mut data = vec![];
    for line in input.lines() {
        let mut arr = vec![];
        for chr in line.unwrap().chars() {
            arr.push(chr);
        }
        data.push(arr);
    }
    return Ok(data);
}

fn count_neighbors(data: &Vec<Vec<char>>, i: isize, j: isize) -> u64 {
    let mut count: u64 = 0;
    for x in (-1isize)..=1 {
        for y in (-1isize)..=1 {
            let a = i+x;
            let b = j+y;
            if !in_range(a, b, data.len(), data[0].len()) {
                continue;
            }
            if data[a as usize][b as usize] == '@' {
                count += 1;
            }
        }
    }
    count - 1
}

#[aoc(4, 1)]
fn p1() ->  Result<u64, String> {
    let input = utils::get_i(4)?;
    let data = parse_data_to_vec(input)?;
    let mut sum = 0;

    for (i, arr) in data.iter().enumerate() {
        for (j, chr) in arr.iter().enumerate() {
            if *chr == '@' {
                if count_neighbors(&data, i as isize, j as isize) < 4 {
                    sum += 1;
                }
            }
        }
    }

    Ok(sum)
}

fn parse_data_to_grid(input: BufReader<File>) -> Result<Grid, String> {
    let mut data = vec![];
    for line in input.lines() {
        let mut arr = vec![];
        for chr in line.unwrap().chars() {
            arr.push(if chr == '@' {TileState::Pending} else {TileState::Empty});
        }
        data.push(arr);
    }
    Ok(Grid{
        width: data.len(),
        height: data[0].len(),
        data: data,
    })
}

struct Grid {
    data: Vec<Vec<TileState>>,
    width: usize,
    height: usize
}

struct TileNeighborIterator {
    x: isize,
    y: isize,
    width: usize,
    height: usize,
    i: isize,
    j: isize
}

trait ToCoords: std::fmt::Debug {
    fn to_coords(&self) -> (usize, usize);
}

impl ToCoords for (usize, usize) {
    fn to_coords(&self) -> (usize, usize) {
        *self
    }
}

impl Grid {
    fn iter(&self) -> GridIterator {
        GridIterator {
            width: self.width,
            height: self.height,
            x: 0,
            y: 0,
        }
    }

    fn neighbors(&self, coords: &impl ToCoords) -> TileNeighborIterator {
        let (x, y) = coords.to_coords();
        return TileNeighborIterator::new(x, y, self.width, self.height);
    }
    fn state(&self, coords: &impl ToCoords) -> &TileState {
        let (x, y) = coords.to_coords();
        return &self.data[x][y];
    }
    fn count_pending(&self, coords: &impl ToCoords) -> u64 {
        let mut count = 0;
        for neighbor in self.neighbors(coords) {
            match *self.state(&neighbor) {
                TileState::Pending => count += 1,
                TileState::Empty | TileState::Removed => (),
            }
        }
        return count;
    }
    fn set_state(&mut self, coords: &impl ToCoords, state: TileState) {
        let (x, y) = coords.to_coords();
        self.data[x][y] = state;
    }
}

struct GridIterator {
    width: usize,
    height: usize,
    x: usize,
    y: usize,
}

impl Iterator for GridIterator {
    type Item = (usize, usize);

    fn next(&mut self) -> Option<Self::Item> {
        if self.x >= self.width {
            return None;
        }
        let coords = (self.x, self.y);
        self.y += 1;
        if self.y == self.height {
            self.y = 0;
            self.x += 1;
        }
        Some(coords)
    }
}

impl TileNeighborIterator {
    fn next_index(&mut self) -> bool {
        self.j += 1;
        if self.j > 1 {
            self.j = -1;
            self.i += 1;
        }

        if self.i == 0 && self.j == 0 {
            // Skip tile itself
            return self.next_index();
        }
        return self.i <= 1;
    }
    fn new(x: usize, y: usize, width: usize, height: usize) -> Self {
        return Self{
            x: x as isize,
            y: y as isize,
            width: width,
            height: height,
            i: -1,
            j: -2
        }
    }
}

impl Iterator for TileNeighborIterator {
    type Item = (usize, usize);

    fn next(&mut self) -> Option<Self::Item> {
        // Get next valid index
        loop {
            if !self.next_index() {
                return None
            }
            if in_range(self.x + self.i, self.y + self.j, self.height, self.width) {
               break;
            }
        }
        return Some(((self.x + self.i) as usize, (self.y + self.j) as usize))
    }
}


#[derive(PartialEq, Eq)]
enum TileState {
    Empty,
    Pending,
    Removed
}

fn process_tile(grid: &mut Grid, tile: impl ToCoords) {
    match grid.state(&tile) {
        TileState::Pending => {
            // Update the tile if its status changed
            if grid.count_pending(&tile) < 4 {
                grid.set_state(&tile, TileState::Removed);
                // Update all tiles around it
                for neighbor in grid.neighbors(&tile) {
                    if *grid.state(&neighbor) == TileState::Pending {
                        process_tile(grid, neighbor);
                    }
                }
            }
        },
        TileState::Removed => (),
        TileState::Empty => (),
    }
}


// #[aoc(4, 2)]
#[allow(dead_code)]
fn p2() ->  Result<u64, String> {
    let input = utils::get_i(4)?;
    let mut grid = parse_data_to_grid(input)?;
    let mut sum = 0;

    for tile in grid.iter() {
        process_tile(&mut grid, tile);
    }

    for tile in grid.iter() {
        if *grid.state(&tile) == TileState::Removed {
            sum += 1;
        }
    }

    Ok(sum)
}
    
#[aoc(4, 2)]
fn p2_nonrecursive() -> Result<u64, String> {
    let input = utils::get_i(4)?;
    let mut grid = parse_data_to_grid(input)?;
    let mut queue = VecDeque::new();
    let mut removed = 0;

    for tile in grid.iter() {
        match grid.state(&tile) {
            TileState::Pending => {
                if grid.count_pending(&tile) < 4 {
                    queue.push_back(tile);
                    grid.set_state(&tile, TileState::Removed);
                }
            },
            _ => ()
        }
    } 
    while let Some(tile) = queue.pop_front() {
        removed += 1;
        for neighbor in grid.neighbors(&tile) {
            match grid.state(&neighbor) {
                TileState::Pending => {
                    if grid.count_pending(&neighbor) < 4 {
                        queue.push_back(neighbor);
                        grid.set_state(&neighbor, TileState::Removed);
                    }
                },
                _ => ()
            }
        }
    }

    Ok(removed)
}