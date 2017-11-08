library(reshape2)
library(ggplot2)
library(scales)
library(extrafont)
library(gridExtra)

source("src/plots/utils.r")

progress_solvers <- data.frame()

for (problem in c("EDFC", "ADFC")) {
  path <- sprintf("results/%s-progress/", problem)

  result_files <- list.files(path=path,
                            pattern=".*.csv")

  for (csv_file in result_files) {
    original_datas <- read.csv(paste(path, csv_file, sep=""), header=T)
    original_datas <- original_datas[original_datas[, "score"] != Inf, ]

    original_datas$Time <- (original_datas$Time - min(original_datas$Time) + 1)/1000

    ## create rows (Time, variable, value), to merge datasets after
    obj_func <- melt(original_datas,
                    ## value.name = obj_func_names["EDFC"][[1]],
                    id.vars = "Time")

    config <- get_config(csv_file)

    obj_func$K <- config["K"]
    obj_func$N <- config["N"]
    obj_func$problem <- problem
    obj_func$solver <- strsplit(csv_file, "-")[[1]][1]

    progress_solvers <- rbind(progress_solvers, obj_func)
  }
}

progress_solvers$letter <- apply(progress_solvers[, c("K", "N")], 1, get_letter)

## normalize objective functions for each configuration and each problem
## to plot nicely (y scale is lost this way tough)
for (letter in unique(progress_solvers$letter)) {
  for (problem in unique(progress_solvers$problem)) {
    rows = progress_solvers$letter == letter & progress_solvers$problem == problem

    progress_solvers[rows, ]$value <- progress_solvers[rows, ]$value / max(progress_solvers[rows, ]$value)
  }
}

for (problem in c("EDFC", "ADFC")) {
  current_plot <- ggplot(
    data=progress_solvers[progress_solvers$problem == problem, ],
    aes(x = Time,
        y = value,
        group = interaction(problem, solver),
        color = interaction(problem, solver, sep=", "),
        )) +
    geom_line() +
    facet_wrap(~ letter, ncol = 2, labeller = as_labeller(format_configs)) +
    scale_x_continuous(trans = log_trans(),
                       breaks = trans_breaks('log10', function(x) 10^x),
                       labels = trans_format('log10', math_format(10^.x))) +
    xlab("Time (s)") +
    ylab("Objective function") +
    my_theme() +
    theme(
      ## remove y labels: they are inconsistent due to normalization
      axis.text.y=element_blank(),

      ## legend settings: position, lines background, title
      legend.position = c(0.75, 0.1),

      panel.grid.major = element_line(linetype = "dotted"),
      )

  ggsave(plot=current_plot,
         filename=sprintf("report/figures/progress/%s.png", problem),
         width=15.5,
         height=20,
         unit="cm",
         device="png")
  ## x11()
  ## print(current_plot)
}
