library(reshape2)
library(ggplot2)
library(scales)
library(extrafont)

get_config = function(name, path="results/simulator/") {
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

my_theme = function() {
  theme(
    ## set everything to serif, we are good people after all
    text = element_text("Charter"),

    ## axis title size
    axis.title.x = element_text(size = 12, margin = margin(t = 15)),
    axis.title.y = element_text(size = 12, margin = margin(r = 15)),

    ## set subtitles preferences, when facet griding
    strip.background = element_rect(fill="white"),
    strip.text = element_text(size = 10),

    ## plot area setting
    panel.background = element_rect(fill = "white"),
    panel.grid.major = element_line(colour = "#666666",
                                    size = 0.3),

    ## legend settings: lines background, title
    legend.key = element_blank(),
    legend.title = element_blank(),
    legend.text = element_text(size = 10),
    )
  }

## manipulate the dataframe to make plots of correct order, as in table
correct_order_configs <- list(
  list(letter="a", K="1000", N="2000"),
  list(letter="b", K="10", N="100"),
  list(letter="c", K="20", N="100"),
  list(letter="d", K="20", N="200"),
  list(letter="e", K="40", N="200"),
  list(letter="f", K="50", N="500"),
  list(letter="g", K="100", N="1000"))

get_letter <- function(row) {
  K = row[1]
  N = row[2]
  for (config in correct_order_configs) {
    if (config$K == K && config$N == N) {
      return(config$letter)
    }
  }
}

format_configs <- function(letters) {
  lapply(letters, function(letter) {
    for (config in correct_order_configs) {
      if (config$letter == letter) {
        return(
          sprintf("%s) K = %s, N = %s",
                  config$letter,
                  config$K,
                  config$N
                  )
        )
        ## return("prova")
      }
    }
  })
}
