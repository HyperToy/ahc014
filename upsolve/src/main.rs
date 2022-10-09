#![allow(non_snake_case)]

use proconio::*;

fn main() {
    let input = read_input();
}

type P = (i32, i32);
type Rect = ([P; 4], usize);

struct Input {
    N: usize,
    ps: Vec<P>,
}
fn read_input() -> Input {
    input! {
        N: usize, M: usize,
        ps: [(i32, i32); M],
    }
    Input { N, ps }
}