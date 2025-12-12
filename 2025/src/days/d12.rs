use std::{
    fs::File,
    io::{BufRead, BufReader},
};

use crate::utils::{self, grid::Grid};
use aoc_macros::aoc;

enum ParsingState {
    Idle,
    Grid,
    Region,
}

struct Region {
    x: u64,
    y: u64,
    nums: Vec<u64>,
}

impl Region {
    fn area(&self) -> u64 {
        self.x * self.y
    }
}

impl TryFrom<&str> for Region {
    type Error = String;

    fn try_from(value: &str) -> Result<Self, Self::Error> {
        let (rect, nums) = value.split_once(": ").ok_or("Invalid input")?;
        let (x, y) = rect.split_once("x").ok_or("Invalid input")?;
        let (x, y) = (x.parse::<u64>().unwrap(), y.parse::<u64>().unwrap());
        let nums = nums
            .split(" ")
            .map(|n| n.parse::<u64>().map_err(|e| e.to_string()))
            .collect::<Result<Vec<_>, String>>()?;
        Ok(Region { x, y, nums })
    }
}

fn parse_input(input: BufReader<File>) -> Result<(Vec<Grid<char>>, Vec<Region>), String> {
    let lines: Vec<String> = input
        .lines()
        .collect::<Result<_, _>>()
        .map_err(|e| e.to_string())?;

    let mut polygons: Vec<Grid<char>> = vec![];
    let mut parsing_state = ParsingState::Idle;

    let mut current_grid = vec![];
    let mut regions = vec![];
    for line in lines.iter() {
        if line == "" {
            if !current_grid.is_empty() {
                polygons.push(Grid::from_string_vec(
                    std::mem::take(&mut current_grid),
                    |x| x,
                ));
            }
            parsing_state = ParsingState::Idle;
        } else {
            match parsing_state {
                ParsingState::Grid => {
                    current_grid.push(line.as_str());
                }
                ParsingState::Region => {
                    regions.push(Region::try_from(line.as_str())?);
                }
                ParsingState::Idle => match line.strip_suffix(":") {
                    Some(_) => parsing_state = ParsingState::Grid,
                    None => {
                        parsing_state = ParsingState::Region;
                        regions.push(Region::try_from(line.as_str())?);
                    }
                },
            }
        }
    }
    Ok((polygons, regions))
}

#[aoc(12, 1)]
fn p1() -> Result<u64, String> {
    let input = utils::get_i(12)?;
    let (polygons, regions) = parse_input(input)?;

    let mut result = 0;

    for region in regions {
        let required_area: u64 = region
            .nums
            .iter()
            .enumerate()
            .map(|(i, count)| polygons[i].count('#') as u64 * count)
            .sum();
        if required_area <= region.area() {
            result += 1;
        }
    }

    Ok(result)
}

#[aoc(12, 2)]
fn p2() -> Result<String, String> {
    Ok("Decorate the North Pole".to_string())
}
