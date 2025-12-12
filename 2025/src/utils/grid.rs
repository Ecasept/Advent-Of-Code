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

impl<T: Clone + Eq> Grid<T> {
    fn iter(&self) -> GridIterator {
        GridIterator {
            width: self.width,
            height: self.height,
            x: 0,
            y: 0,
        }
    }

    fn iter_items(&self) -> impl Iterator<Item = T> + use<'_, T> {
        self.iter().map(|(x, y)| self.data[y][x].clone())
    }

    pub fn from_vec(vec: Vec<Vec<T>>) -> Self {
        Grid {
            height: vec.len(),
            width: vec.first().map_or(0, |row| row.len()),
            data: vec,
        }
    }
    pub fn from_string_vec(vec: Vec<&str>, conv: fn(char) -> T) -> Self {
        let data: Vec<Vec<T>> = vec.iter().map(|s| s.chars().map(conv).collect()).collect();
        Self::from_vec(data)
    }
    pub fn from_buf_read(buf_read: impl BufRead, conv: fn(char) -> T) -> Self {
        let data: Vec<Vec<T>> = buf_read
            .lines()
            .map(|line| line.expect("Malformed lines").chars().map(conv).collect())
            .collect();
        Self::from_vec(data)
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

    pub fn count(&self, el: T) -> usize {
        self.iter_items().filter(|x| *x == el).count()
    }
}
