use std::io::BufRead;

pub struct Grid<T> {
    data: Vec<Vec<T>>,
    pub width: usize,
    pub height: usize,
}

pub trait ToCoords {
    fn to_coords(&self) -> (usize, usize);
}

pub trait ToCoordsI {
    fn to_coords(&self) -> (isize, isize);
}

impl ToCoords for (usize, usize) {
    fn to_coords(&self) -> (usize, usize) {
        *self
    }
}

impl ToCoordsI for (isize, isize) {
    fn to_coords(&self) -> (isize, isize) {
        *self
    }
}

impl ToCoords for (u64, u64) {
    fn to_coords(&self) -> (usize, usize) {
        let val = (
            self.0.try_into().expect("Too large coordinates"),
            self.1.try_into().expect("Too large coordinates"),
        );
        val
    }
}

impl<T> Grid<T> {
    pub fn from_buf_read(buf_read: impl BufRead, conv: fn(char) -> T) -> Self {
        let data: Vec<Vec<T>> = buf_read
            .lines()
            .map(|line| line.expect("Malformed lines").chars().map(conv).collect())
            .collect();

        return Grid {
            height: data.len(),
			width: data.first().map_or(0, |row| row.len()),
            data,
        };
    }

    #[allow(dead_code)]
    pub fn in_range(&self, coords: impl ToCoords) -> bool {
        let (x, y) = coords.to_coords();
        x < self.width && y < self.height
    }

    #[allow(dead_code)]
    pub fn in_range_i(&self, coords: impl ToCoordsI) -> bool {
        let (x, y) = coords.to_coords();
        match (usize::try_from(x), usize::try_from(y)) {
            (Ok(x), Ok(y)) => self.in_range((x, y)),
            _ => false,
        }
    }
    pub fn at(&self, coords: impl ToCoords) -> &T {
        let (x, y) = coords.to_coords();
        &self.data[y][x]
    }
    pub fn rows(&self) -> &Vec<Vec<T>> {
        &self.data
    }
}
