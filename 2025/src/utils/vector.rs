use std::ops::Add;

/// A three-dimensional vector
#[derive(Clone, Copy, Debug)]
pub struct Vector3 {
    pub x: i64,
    pub y: i64,
    pub z: i64,
}

fn vector_from_string(string: &str) -> Result<Vec<i64>, String> {
    string
        .split(",")
        .map(|num| num.trim().parse::<i64>())
        .collect::<Result<_, _>>()
        .map_err(|e| e.to_string())
}

impl Vector3 {
    /// Distance to another vector as a Vector
    pub fn delta(&self, other: Self) -> Vector3 {
        Vector3 {
            x: other.x - self.x,
            y: other.y - self.y,
            z: other.z - self.z,
        }
    }
    /// Non-negative version of the vector
    pub fn abs(&self) -> Vector3 {
        Vector3 {
            x: self.x.abs(),
            y: self.y.abs(),
            z: self.z.abs(),
        }
    }

    /// Euclidean distance to another vector
    pub fn distance_to(&self, other: Self) -> f64 {
        let dif = self.delta(other).abs();
        ((dif.x.pow(2) + dif.y.pow(2) + dif.z.pow(2)) as f64).sqrt()
    }
    pub fn from_string(string: &str) -> Result<Vector3, String> {
        let parts = vector_from_string(string)?;

        match parts.as_slice() {
            [x, y, z] => Ok(Vector3 {
                x: *x,
                y: *y,
                z: *z,
            }),
            _ => Err(format!("Expected 3 elements, found {}", parts.len())),
        }
    }
    pub fn zero() -> Vector3 {
        Vector3 { x: 0, y: 0, z: 0 }
    }
}

#[derive(Copy, Clone, PartialEq, Debug, Eq, PartialOrd, Ord)]
pub struct Vector2 {
    pub x: i64,
    pub y: i64,
}

impl Vector2 {
    pub fn area_with(&self, other: &Self) -> i64 {
        (other.x - self.x).abs().add(1) * (other.y - self.y).abs().add(1)
    }
    pub fn from_string(string: &str) -> Result<Self, String> {
        let parts = vector_from_string(string)?;
        match parts.as_slice() {
            [c1, c2] => Ok(Vector2 { x: *c1, y: *c2 }),
            _ => Err("Could not convert string to vector".to_owned()),
        }
    }
}
