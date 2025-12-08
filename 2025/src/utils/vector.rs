/// A three-dimensional vector
#[derive(Clone, Copy, Debug)]
pub struct Vector3(pub i64, pub i64, pub i64);

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
        let parts: Vec<i64> = string
            .split(",")
            .map(|num| num.trim().parse::<i64>())
            .collect::<Result<_, _>>()
            .map_err(|e| e.to_string())?;

        match parts.as_slice() {
            [x, y, z] => Ok(Vector3(*x, *y, *z)),
            _ => Err(format!("Expected 3 elements, found {}", parts.len())),
        }
    }
    pub fn zero() -> Vector3 {
        Vector3(0, 0, 0)
    }
}
