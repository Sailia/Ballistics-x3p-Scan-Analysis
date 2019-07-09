library(readr)
library(tidyverse)
library(x3ptools)
library(bulletxtrctr)

# Read in data, create new path for files
problem_bullets <- read_csv("data/problem_bullets.csv") %>%
  mutate(file_exists = file.exists(source),
         filename = basename(source) %>%
           str_replace("(Land \\d) ?-.*", "\\1.x3p") %>%
           str_replace_all(" - ", "-") %>%
           str_replace_all("\\s{1,}", "_"),
         scanner = str_extract(source, "Variability/[A-z]{1,}") %>%
           str_remove("Variability/") %>%
           paste0(., "_") %>%
           str_remove("^NA_$"),
         filename = paste0(scanner, filename),
         newpath = file.path("data/problem_scans", class, filename))

purrr::map_lgl(file.path("data/problem_scans", unique(problem_bullets$class)), dir.create, recursive = T)
purrr::map_lgl(file.path("docs/figure/problem_scans", unique(problem_bullets$class)), dir.create, recursive = T)

# Copy x3p files to this project directory for conveninence
file.copy(problem_bullets$source, problem_bullets$newpath, recursive = T)


# Create png images of x3p files
problem_bullets <- problem_bullets %>% #filter(row_number() == 2) %>%
  mutate(x3p = purrr::map(newpath, read_x3p),
         image_file = purrr::map2(x3p, newpath,
                                  ~x3p_image(.x, file = str_replace_all(.y, c("data" = "docs/figure",
                                                                              "x3p" = "png")),
                                             size = c(3000, 1000))
         ))




# Read in data, create new path for files
good_bullets <- read_csv("data/good_bullets.csv") %>%
  mutate(file_exists = file.exists(source),
         filename = basename(source) %>%
           str_replace("(Land \\d) ?-.*", "\\1.x3p") %>%
           str_replace_all(" - ", "-") %>%
           str_replace_all("\\s{1,}", "_"),
         scanner = str_extract(source, "Variability/[A-z]{1,}") %>%
           str_remove("Variability/") %>%
           paste0(., "_") %>%
           str_remove("^NA_$"),
         filename = paste0(scanner, filename),
         class = "good",
         newpath = file.path("data/good_scans", class, filename))

purrr::map_lgl(file.path("data/good_scans", unique(good_bullets$class)), dir.create, recursive = T)
purrr::map_lgl(file.path("docs/figure/good_scans", unique(good_bullets$class)), dir.create, recursive = T)

# Copy x3p files to this project directory for conveninence
file.copy(good_bullets$source, good_bullets$newpath, recursive = T)


# Create png images of x3p files
good_bullets <- good_bullets %>% #filter(row_number() == 2) %>%
  mutate(x3p = purrr::map(newpath, read_x3p),
         image_file = purrr::map2(x3p, newpath,
                                  ~x3p_image(.x, file = str_replace_all(.y, c("data" = "docs/figure",
                                                                              "x3p" = "png")),
                                             size = c(3000, 1000))
         ))



