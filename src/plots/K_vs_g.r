library(reshape2)
library(ggplot2)
library(scales)
library(extrafont)
library(gridExtra)

source("src/plots/utils.r")

dataset = read.csv("results/simulator/optimal-configuration.csv", header=T)

etas = seq(1, 2.5, length=10)
path="results/simulator/"

eta_probs <- data.frame()
result_files <- list.files(path=path,
                          pattern="etas.*.csv")

for (csv_file in result_files) {
  probs = read.csv(paste(path, csv_file, sep=""), header=FALSE)
  config = get_config(csv_file)

  ## if current configuration lead to no decoding (prob = 0) go on
  if (sum(probs) == 0) {
    next
  }

  # add row for each probability value
  for (index in 1:dim(probs)[1]) {
    current_eta_probs = data.frame(K = config["K"],
                                   N = config["N"],
                                   eta = etas[index],
                                   prob = probs[[1]][index])

    eta_probs = rbind(current_eta_probs, eta_probs)
  }
}

dataset = merge(dataset, eta_probs)

## compute number of transmissions performed globally
dataset$number.of.transmissions = dataset$Random.walk.length * dataset$Number.of.packets

y_padding = 10**7
N_plot = ggplot(data=dataset, aes(x=N,
                                  y=prob,
                                  color=eta,
                                  group=eta,
                                  )) +
  my_theme() +
  geom_point()
  ## facet_wrap(~eta, ncol=4)

N_plot
