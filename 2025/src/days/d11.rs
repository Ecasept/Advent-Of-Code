use std::collections::HashMap;
use std::io::Read;

use crate::utils;
use aoc_macros::aoc;

type Node<'a> = &'a str;
type NodeGraph<'a> = HashMap<Node<'a>, Vec<Node<'a>>>;

fn dfs_memo<'a>(node: Node, graph: &NodeGraph<'a>, memo: &mut HashMap<Node<'a>, u64>) -> u64 {
    match node {
        "out" => 1,
        _ => graph
            .get(node)
            .unwrap()
            .iter()
            .map(|n| match memo.get(n) {
                Some(x) => *x,
                None => {
                    let val = dfs_memo(*n, graph, memo);
                    memo.insert(*n, val);
                    val
                }
            })
            .sum(),
    }
}

#[aoc(11, 1)]
fn p1() -> Result<u64, String> {
    let mut input = String::new();
    utils::get_i(11)?
        .read_to_string(&mut input)
        .map_err(|e| e.to_string())?;

    let mut graph = HashMap::new();
    for line in input.lines() {
        let (node, edges) = line.split_once(": ").ok_or("Invalid input".to_owned())?;
        graph.insert(node, edges.split(" ").collect::<Vec<&str>>());
    }

    let mut memo = HashMap::new();
    Ok(dfs_memo("you", &graph, &mut memo))
}

fn dfs_memo2<'a>(
    node: Node,
    graph: &NodeGraph<'a>,
    memo: &mut HashMap<Node<'a>, (u64, u64, u64, u64)>,
) -> (u64, u64, u64, u64) {
    match node {
        "out" => (
            0, // ways to get from this node to "out" without having found anything
            0, // ways to get if you have found a dac
            0, // ways to get if you have found an fft
            1, // ways to get to out if you have found dac and fft
        ),
        _ => graph
            .get(node)
            .unwrap()
            .iter()
            // Compute for neighbor
            .map(|neighbor| match memo.get(neighbor) {
                Some(x) => *x,
                None => {
                    let val = dfs_memo2(*neighbor, graph, memo);
                    memo.insert(*neighbor, val);
                    val
                }
            })
            .map(|mut num| match node {
                "dac" => {
                    // If you you previously needed a dac and fft, now u just need an fft
                    num.2 += num.3;
                    num.3 = 0;
                    // If you previously needed a dac, now you don't need anyting
                    num.0 += num.1;
                    num.1 = 0;
                    num
                }
                "fft" => {
                    // If you you previously needed a dac and fft, now u just need a dac
                    num.1 += num.3;
                    num.3 = 0;
                    // If you previously needed an fft, now you don't need anyting
                    num.0 += num.2;
                    num.2 = 0;
                    num
                }
                // You still need the same things
                _ => num,
            })
            // Add up the ways from all neighbors
            .fold((0, 0, 0, 0), |acc, v| {
                (acc.0 + v.0, acc.1 + v.1, acc.2 + v.2, acc.3 + v.3)
            }),
    }
}

#[aoc(11, 2)]
fn p2() -> Result<u64, String> {
    let mut input = String::new();
    utils::get_i(11)?
        .read_to_string(&mut input)
        .map_err(|e| e.to_string())?;

    let mut graph = HashMap::new();
    for line in input.lines() {
        let (node, edges) = line.split_once(": ").ok_or("Invalid input".to_owned())?;
        graph.insert(node, edges.split(" ").collect::<Vec<&str>>());
    }

    let mut memo = HashMap::new();
    Ok(dfs_memo2("svr", &graph, &mut memo).0)
}
