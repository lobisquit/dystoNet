library(reshape2)
library(ggplot2)
library(scales)
library(extrafont)

source("src/plots/utils.r")

path="results/simulator/"
etas = seq(1, 2.5, length=10)

## read all simulator results
eta_probs <- data.frame()
result_files <- list.files(path="results/simulator/", pattern="etas.*.csv")
for (csv_file in result_files) {
  probs = read.csv(paste(path, csv_file, sep=""), header=FALSE)
  config = get_config(csv_file)

  ## if current configuration lead to no decoding (prob = 0)
  ## go on
  if (sum(probs) == 0) {
    next
  }

  print(config)

  # add row for each probability value
  for (index in 1:dim(probs)[1]) {
    current_eta_probs = data.frame(K = config["K"],
                                   N = config["N"],
                                   problem = config["problem"],
                                   solver = config["solver"],
                                   eta = etas[index],
                                   prob = probs[[1]][index])

    eta_probs = rbind(current_eta_probs, eta_probs)
  }
}

## map each couple N, K to the corresponding letter
eta_probs$letter = apply(eta_probs[,c("K", "N")], 1, get_letter)

## make two distinct plots: one for ADFC only, to show only GA is reliable
## and one with all EDFC and the best ADFC, GA

portions = list(
  "eta_vs_prob_adfc_only" = eta_probs[eta_probs$problem == "ADFC",],
  "eta_vs_prob_reliable_ones" = eta_probs[eta_probs$problem == "EDFC" | eta_probs$solver == "GA",]
)

plots = list()
for (portion in names(portions)) {
  plot = ggplot(data=portions[portion][[1]],
                aes(x=eta,
                    y=prob,
                    group=interaction(problem, solver),
                    color=interaction(problem, solver, sep=", "),
                    )) +
    geom_line() +
    facet_wrap(~ letter, ncol = 2, labeller = as_labeller(format_configs)) +
    my_theme() +
    theme(
      ## rotate x ticks lables
      axis.text.x = element_text(angle = 45, hjust = 1),

      ## legend settings: position, lines background, title
      legend.position = c(0.75, 0.1),

      panel.grid.major = element_line(linetype = "dotted"),
      ) +
  labs(x = "Decoding ratio", y = "Decoding probability") +
  scale_x_continuous(breaks = etas,
                     labels = function(x) format(round(x, 2), nsmall = 2)) +
  scale_colour_brewer(palette="Set1") +
  guides(col = guide_legend(ncol = 1))

  ## show and save plot
  plot
  loadfonts()
  ggsave(plot=plot,
         filename=sprintf("report/figures/%s.pdf", portion),
         width=15.5,
         height=20,
         unit="cm",
         device="pdf")
}
