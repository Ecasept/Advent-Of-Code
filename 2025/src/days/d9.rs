use std::io::BufRead;

use crate::utils::{self, vector::Vector2};
use aoc_macros::aoc;

#[aoc(9, 1)]
fn p1() -> Result<u64, String> {
    let input = utils::get_i(9)?;
    let points = input
        .lines()
        .map(|line| {
            let line = line.map_err(|e| e.to_string())?;
            Vector2::from_string(line.as_str())
        })
        .collect::<Result<Vec<Vector2>, String>>()?;
    let mut areas: Vec<(usize, usize, i64)> = points
        .iter()
        .enumerate()
        .map(|(i, p)| {
            points
                .iter()
                .enumerate()
                .filter(move |(j, _)| *j > i)
                .map(move |(j, q)| (i, j, p.area_with(q)))
        })
        .flatten()
        .collect();
    areas.sort_unstable_by_key(|a| a.2);
    Ok(areas.last().unwrap().2 as u64)
}

fn contiuous_pairs<T: Ord + Copy>(vec: &Vec<T>) -> impl Iterator<Item = (T, T)> + '_ {
    (0..(vec.len())).map(|i| (vec[i], vec[(i + 1) % vec.len()]))
}

fn point_in_polygon(point: Vector2, pol: &Vec<Vector2>) -> bool {
    let (x, y) = (point.0, point.1);
    let n = pol.len();
    let mut inside = false;

    let mut p1 = pol[0];

    for i in 1..=n {
        let p2 = pol[i % n];

        // Check if horizontal ray from point crosses this edge
        if y > p1.1.min(p2.1) {
            if y <= p1.1.max(p2.1) {
                if x <= p1.0.max(p2.0) {
                    if p1.1 != p2.1 {
                        let xinters = (y - p1.1) * (p2.0 - p1.0) / (p2.1 - p1.1) + p1.0;
                        if p1.0 == p2.0 || x <= xinters {
                            inside = !inside;
                        }
                    }
                }
            }
        }

        p1 = p2;
    }

    inside
}

fn line_intersect(point1: (Vector2, Vector2), point2: (Vector2, Vector2)) -> bool {
    let (p1, p2) = point1;
    let (p3, p4) = point2;
    /// Check if all three poitns are counter-clockwise
    fn ccw(a: Vector2, b: Vector2, c: Vector2) -> bool {
        (c.1 - a.1) * (b.0 - a.0) > (b.1 - a.1) * (c.0 - a.0)
    }
    ccw(p1, p3, p4) != ccw(p2, p3, p4) && ccw(p1, p2, p3) != ccw(p1, p2, p4)
}

fn polygon_inside(pol1: &Vec<Vector2>, pol2: &Vec<Vector2>) -> bool {
    if contiuous_pairs(pol1)
        .all(|pair| contiuous_pairs(pol2).all(|pair2| !line_intersect(pair, pair2)))
    {
        if point_in_polygon(pol2[0], pol1) {
            return true;
        }
    }
    return false;
}

#[aoc(9, 2)]
fn p2() -> Result<u64, String> {
    let input = utils::get_i(9)?;
    let points = input
        .lines()
        .map(|line| {
            let line = line.map_err(|e| e.to_string())?;
            Vector2::from_string(line.as_str())
        })
        .collect::<Result<Vec<Vector2>, String>>()?;
    let mut areas: Vec<(usize, usize, i64)> = points
        .iter()
        .enumerate()
        .map(|(i, p)| {
            points
                .iter()
                .enumerate()
                .filter(move |(j, _)| *j > i)
                .map(move |(j, q)| (i, j, p.area_with(q)))
        })
        .flatten()
        .collect();
    areas.sort_unstable_by_key(|a| a.2);

    let found = areas
        .iter()
        .rev()
        .position(|(i, j, _)| {
            let mut p1 = points[*i];
            let mut p2 = points[*j];
            if p1.1 > p2.1 {
                // Make p1 the lower one
                std::mem::swap(&mut p1, &mut p2);
            }
            // Make square a bit smaller
            if p1.0 < p2.0 {
                p1.0 += 1;
                p1.1 += 1;
                p2.0 -= 1;
                p2.1 -= 1;
            } else {
                p1.0 -= 1;
                p1.1 += 1;
                p2.0 += 1;
                p2.1 -= 1;
            }
            let p3 = Vector2(p1.0, p2.1);
            let p4 = Vector2(p2.0, p1.1);
            return polygon_inside(&points, &vec![p1, p2, p3, p4]);
        })
        .unwrap();

    Ok(areas[areas.len() - 1 - found].2 as u64)
}
