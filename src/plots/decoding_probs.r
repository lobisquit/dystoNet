
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

  # add row for each probability value
  for (index in 1:dim(probs)[1]) {
    problem_solver = paste(config["problem"], config["solutor"], sep=", ")

    current_eta_probs = data.frame(K = config["K"],
                                   N = config["N"],
                                   problem = config["problem"],
                                   solutor = config["solutor"],
                                   eta = etas[index],
                                   prob = probs[[1]][index])

    eta_probs = rbind(current_eta_probs, eta_probs)
  }
}

## manipulate the dataframe to make plots of correct order
correct_order_configs = list(
  list(letter="a", K=1000, N=2000),
  list(letter="b", K=10, N=100),
  list(letter="c", K=20, N=100),
  list(letter="d", K=20, N=200),
  list(letter="e", K=40, N=200),
  list(letter="f", K=50, N=500),
  list(letter="g", K=100, N=1000))

## map each couple N, K to the corresponding letter
eta_probs$letter = apply (eta_probs[,c("K", "N")], 1, function(row) {
  K = row[1]
  N = row[2]
  for (config in correct_order_configs) {
    if (config$K == K && config$N == N) {
      return(config$letter)
    }
  }
})

final_p = ggplot(data=eta_probs, aes(x=eta,
                                     y=prob,
                                     group=interaction(problem, solutor),
                                     color=interaction(problem, solutor, sep=", "),
                                     )) +
  geom_line() +
  facet_wrap(~ letter, ncol = 2, labeller = function(letters) {
    ## look up information about given configuration
    ## and build a proper title
    lapply(letters, function(letter) {
      for (config in correct_order_configs) {
        if (config$letter == letter) {
          return(sprintf("%s) K = %s, N = %s",
                    config$letter,
                    config$K,
                    config$N))
        }
      }
    })
  }) +
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
  scale_colour_brewer(palette="Set1")

## plot also on window
final_p

## load all extrafonts
loadfonts()
ggsave(plot=final_p,
       filename="report/figures/eta_vs_prob.pdf",
       width=15.5,
       height=20,
       unit="cm",
       device="pdf")
