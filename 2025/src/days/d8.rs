use std::collections::HashMap;
use std::io::BufRead;

use crate::utils;
use crate::utils::vector::Vector3;
use aoc_macros::aoc;

#[aoc(8, 1)]
fn p1() -> Result<u64, String> {
    let input = utils::get_i(8)?;
    let points: Vec<(usize, Vector3)> = input
        .lines()
        .enumerate()
        .map(|(i, line)| {
            let line = line.map_err(|e| e.to_string())?;
            let point = Vector3::from_string(&line)?;
            Ok::<(usize, Vector3), String>((i, point))
        })
        .collect::<Result<_, _>>()?;
    let mut groups: Vec<usize> = (0_usize..(points.len())).collect();

    let mut edges: Vec<(usize, usize, f64)> = points
        .iter()
        .map(|(i, p)| points.iter().map(|(j, q)| (*i, *j, p.distance_to(*q))))
        .flatten()
        .filter(|(i, j, _)| i > j)
        .collect();
    edges.sort_unstable_by(|x, y| x.2.partial_cmp(&y.2).unwrap());

    let mut i = 0;
    for (fi, ti, _) in edges {
        if i == 1000 {
            break;
        }
        let g1 = groups[fi];
        let g2 = groups[ti];

        if g1 == g2 {
            i += 1;
            continue;
        } else {
            i += 1;
            groups.iter_mut().for_each(|v| {
                if *v == g2 {
                    *v = g1
                }
            });
        }
    }

    let mut counts = HashMap::new();

    for num in groups {
        *counts.entry(num).or_insert(0) += 1;
    }

    let mut counts: Vec<_> = counts.into_iter().collect();
    counts.sort_by_key(|&(_, count)| std::cmp::Reverse(count));

    Ok(counts.iter().take(3).fold(1, |acc, (_, x)| acc * x))
}

#[aoc(8, 2)]
fn p2() -> Result<u64, String> {
    let input = utils::get_i(8)?;
    let points: Vec<(usize, Vector3)> = input
        .lines()
        .enumerate()
        .map(|(i, line)| {
            let line = line.map_err(|e| e.to_string())?;
            let point = Vector3::from_string(&line)?;
            Ok::<(usize, Vector3), String>((i, point))
        })
        .collect::<Result<_, _>>()?;
    let mut groups: Vec<usize> = (0_usize..(points.len())).collect();

    let mut edges: Vec<(usize, usize, f64)> = points
        .iter()
        .map(|(i, p)| points.iter().map(|(j, q)| (*i, *j, p.distance_to(*q))))
        .flatten()
        .filter(|(i, j, _)| i > j)
        .collect();

    edges.sort_unstable_by(|x, y| x.2.partial_cmp(&y.2).unwrap());

    let mut group_count = points.len();

    let mut last_from: Vector3 = Vector3::zero();
    let mut last_to: Vector3 = Vector3::zero();

    for (fi, ti, _) in edges {
        let g1 = groups[fi];
        let g2 = groups[ti];

        // println!("Counnecting {:?} to {:?}", from, to);

        if g1 == g2 {
            // println!(" Skipping");
            continue;
        } else {
            groups.iter_mut().for_each(|v| {
                if *v == g2 {
                    *v = g1
                }
            });
            group_count -= 1;
            if group_count == 1 {
                last_from = points[fi].1;
                last_to = points[ti].1;
                break;
            }
        }
    }
    Ok((last_from.x * last_to.x) as u64)
}
