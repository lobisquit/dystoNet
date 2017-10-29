library(reshape2)
library(ggplot2)
library(scales)
library(extrafont)

path="results/simulator/"
get_config = function(name) {
  tokens = strsplit(name, "\\.csv|-")
  # remove first element from returned list
  tokens = tokens[[1]][-1]

  info = list()
  for (element in tokens) {
    name <- strsplit(element, "=")[[1]][1]
    value <- strsplit(element, "=")[[1]][2]
    info[name] = value
  }
  info
}

etas = seq(1, 2.5, length=10)

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
    configuration = paste(paste("K", config["K"], sep="="),
                          paste("N", config["N"], sep="="),
                          ## paste("c", config["c"], sep="="),
                          ## paste("delta", config["delta"], sep="="),
                          sep=", ")

    problem_solver = paste(config["problem"], config["solutor"], sep=" - ")

    current_eta_probs = data.frame(config = configuration,
                                   problem_solver = problem_solver,
                                   eta = etas[index],
                                   prob = probs[[1]][index]
                                   )

    eta_probs = rbind(current_eta_probs, eta_probs)
  }
}

## manipulate the dataframe to make plots of correct order
correct_order_configs = c(
  "K=1000, N=2000",
  "K=10, N=100",
  "K=20, N=100",
  "K=20, N=200",
  "K=40, N=200",
  "K=50, N=500",
  "K=100, N=1000"
)
eta_probs = within(eta_probs,
                   config <- factor(config, levels=correct_order_configs))

## put letters before configurations, in order
letters = c("a", "b", "c", "d", "e", "f", "g", "h")
for (index in 1:length(correct_order_configs)) {
  correct_config = correct_order_configs[index]

  ## put proper letter in front of network configuration
  ## put spaces aroung equals
  eta_probs$marked_config[eta_probs$config == correct_config] <-
    gsub("=", " = ",
         paste(letters[index], correct_config, sep=") "))
}

final_p = ggplot(data=eta_probs, aes(x=eta,
                                     y=prob,
                                     group=problem_solver,
                                     color=factor(problem_solver))) +
  geom_line() +
  facet_wrap(~ marked_config, ncol = 2) +
  theme(
    ## set everything to serif, we are good people after all
    text = element_text("Charter"),

    ## axis title size
    axis.title.x = element_text(size = 12, margin = margin(t = 15)),
    axis.title.y = element_text(size = 12, margin = margin(r = 15)),

    ## rotate x ticks lables
    axis.text.x = element_text(angle = 45, hjust = 1),

    ## set subtitles preferences
    strip.background = element_rect(fill="white"),
    strip.text = element_text(size = 10),

    ## plot area setting
    panel.background = element_rect(fill = "white"),
    panel.grid.major = element_line(colour = "#666666",
                                    linetype = "dotted",
                                    size = 0.3),

    ## legend settings: position, lines background, title
    legend.position = c(0.75, 0.1),
    legend.key = element_blank(),
    legend.title = element_blank(),
    legend.text = element_text(size = 10),
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
