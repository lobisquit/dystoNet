# dystoNet
Course project of Wireless Communication.

## Aim
Project aim is to assess performance of linear network coding,
performed using *Fountain Codes*.

This procedure is outlined in our reference paper.
```
Y. Lin, B. Liang, and B. Li, “Data persistence in large-scale sensor networks with decentralized fountain codes”
```

## Structure
Main file to handle the project is `make.py`, that automates compilation, running, docs creation of each of our packages, i.e. `src/` sub-directories.

### build/
Object files and executable are stored here temporarily, from one execution to another.

### doc/
Doxygen documentation of C++ code is created here running
```
python make.py --mode docs
```

### report/
LaTeX directory for project report.

### results/
Computations results (mainly csv files) are stored here.

### src/
Each package, i.e. `src/` sub-directory, contains a `main.cpp` file, that is compiled and run setting the `--package` parameter of `make.py`.

Dependencies are added with the `--deps` package list, while specific configuration parameters are retrieved by main `config.json` file, with `--config` option.

Example here is given to run simulated annealing and jumping ball on first problem (EDFC).
```
python make.py --mode run --package first_problem/simulated_annealing --deps first_problem utils --configs configs.json
python make.py --mode run --package first_problem/jumping_ball --deps first_problem/simulated_annealing first_problem utils --configs configs.json
```

### src/plots/
Unlike other `src/` sub-directories, this is not a C++ package but contains some R code to plot results in csv files.
Those scripts should be run from project root directory.

### test/
Each subdirectory of this folder contains a bunch of tests for a package (of `src/`). They can be run giving, for package `utils`, this command
```
python make.py --mode test --package utils
```

## Documentation
Documentation is automatically created using Doxygen and can be retrieved [here](https://codedocs.xyz/lobisquit/dystoNet/).
