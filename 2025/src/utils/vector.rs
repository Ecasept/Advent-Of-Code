use std::ops::Add;

/// A three-dimensional vector
#[derive(Clone, Copy, Debug)]
pub struct Vector3(pub i64, pub i64, pub i64);

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
        Vector3(other.0 - self.0, other.1 - self.1, other.2 - self.2)
    }
    /// Non-negative version of the vector
    pub fn abs(&self) -> Vector3 {
        Vector3(self.0.abs(), self.1.abs(), self.2.abs())
    }

    /// Euclidean distance to another vector
    pub fn distance_to(&self, other: Self) -> f64 {
        let dif = self.delta(other).abs();
        ((dif.0.pow(2) + dif.1.pow(2) + dif.2.pow(2)) as f64).sqrt()
    }
    pub fn from_string(string: &str) -> Result<Vector3, String> {
        let parts = vector_from_string(string)?;

        match parts.as_slice() {
            [x, y, z] => Ok(Vector3(*x, *y, *z)),
            _ => Err(format!("Expected 3 elements, found {}", parts.len())),
        }
    }
    pub fn zero() -> Vector3 {
        Vector3(0, 0, 0)
    }
}

#[derive(Copy, Clone, PartialEq, Debug, Eq, PartialOrd, Ord)]
pub struct Vector2(pub i64, pub i64);

impl Vector2 {
    pub fn area_with(&self, other: &Self) -> i64 {
        (other.0 - self.0).abs().add(1) * (other.1 - self.1).abs().add(1)
    }
    pub fn from_string(string: &str) -> Result<Self, String> {
        let parts = vector_from_string(string)?;
        match parts.as_slice() {
            [c1, c2] => Ok(Vector2(*c1, *c2)),
            _ => Err("Could not convert string to vector".to_owned()),
        }
    }
}
