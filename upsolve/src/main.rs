#![allow(non_snake_case)]

use proconio::*;

fn main() {
    let input = read_input();
    let output = solve(&input);
    write_output(&output);
}

fn solve(input: &Input) -> Output {
    let mut best = vec![];
    best
}

type P = (i32, i32);
type Rect = ([P; 4], usize);
type Output = Vec<Rect>;

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

fn write_output(output: &Output) {
    println!("{}", output.len());
    for rect in output {
        for i in 0..4 {
            println!("{} {}", rect.0[i].0, rect.0[i].1);
            if i == 3 {
                println!();
            } else {
                println!(" ");
            }
        }
    }
}