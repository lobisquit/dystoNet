import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib import rc, rcParams

# plt.rcParams['savefig.facecolor']='red'
rcParams['font.family'] = 'serif'
rcParams['grid.linestyle'] = ':'
rc('text', usetex=True)

RESULT_DIR = Path('results/simulator')

def parse_result_file(result_file):
	# remove file name and split using hyphens
	tokens = result_file.name.replace('.csv', '').split('-')

	problem = tokens[1].split('=')[1]
	solutor = tokens[2].split('=')[1]
	K = int(tokens[3].split('=')[1])
	N = int(tokens[4].split('=')[1])
	c = float(tokens[5].split('=')[1])
	delta = float(tokens[6].split('=')[1])

	return {
		"problem" : problem,
		"solutor" : solutor,
		"K" : K,
		"N" : N,
		"c" : c,
		"delta" : delta
	}

probs = {}

for result_file in RESULT_DIR.glob('etas*.csv'):
	configs = parse_result_file(result_file)

	# decoding probability varying eta for given configuration
	p = pd.read_csv(result_file, header=None)

	p.columns = ['{} - {}'.format(configs['problem'], configs['solutor'])]
	p = p.set_index(np.linspace(1, 2.5, 10))

	# note that double {{}} excapes format
	tag = 'K={}, N={}, $\\delta={}$'.format(
		configs["K"],
		configs["N"],
		configs["delta"])

	# add to global results only if solution works
	# (i.e. probability of success is not always 0)
	if p.values.sum() != 0:
		# add to probabilities dict or merge with existing one
		if tag in probs:
			probs[tag] = pd.concat([probs[tag], p], axis=1)
		else:
			probs[tag] = p
	else:
		print("Success probability was always 0!")

problem_solutors = np.unique(np.concatenate(
	[scores.columns for config, scores in probs.items()]))

colors = dict(zip(
	problem_solutors,
	plt.cm.viridis(np.linspace(0, 1, len(problem_solutors)))))

# plot decoding probability vs decoding ratio for various c, for each K, N, delta configuration

lines = []
labels = []
figure_size = (3, 2.2)
for config, scores in probs.items():
	fig = plt.figure(config, figsize=figure_size)
	ax = fig.gca()

	for column in sorted(scores.columns):
		line, = ax.plot(scores.index, scores[column], color=colors[column], label=column)
		if len(lines) < len(scores.columns):
			lines.append(line)
			labels.append(column)

	# ax.set_title(config)
	ax.set_xlabel("Decoding ratio $\\eta$")
	ax.set_ylabel("Decoding probability")
	ax.grid(True)

	plt.tight_layout()

	fig.savefig(
		"report/figures/eta_vs_prob/plot-" +
		config.replace(" ", "")
		      .replace(".", "")
		      .replace("$", "")
		      .replace(",", "-")
		      .replace("\\", "") +
		".eps", format='eps', transparent=True)
	# plt.show()

# show legend by itself
figlegend = plt.figure(figsize=figure_size)
figlegend.legend(lines, labels, 'center')

figlegend.savefig("report/figures/eta_vs_prob/legend.eps", format='eps')
# plt.show()
