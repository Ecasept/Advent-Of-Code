pub mod d1;

pub fn execute(day: u8) -> Result<(), String> {
    let p1: i64;
    let p2: i64;
    match day {
        1 => {
            p1 = d1::day1_part1()?;
            p2 = d1::day1_part2()?;
        }
        _ => return Err("Day not found!".to_owned()),
    }
    println!("Day {day} Part 1: {}", p1);
    println!("Day {day} Part 2: {}", p2);
    return Ok(());
}
