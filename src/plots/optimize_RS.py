from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib import rc

rc('text', usetex=True)

RESULT_DIR = Path('../../results/optimize_c')

def parse_result_file(result_file):
	# remove file name and split using hyphens
	tokens = result_file.name.replace('.csv', '').split('-')
	print(tokens)
	K = int(tokens[1].split('=')[1])
	N = int(tokens[2].split('=')[1])
	c = float(tokens[3].split('=')[1])

	etas_delimiters = tokens[4].split('=')[1].split(',')
	etas = np.linspace(
		float(etas_delimiters[0]),
		float(etas_delimiters[1]),
		int(etas_delimiters[2]))

	delta = float(tokens[5].split('=')[1])

	return {
		'K': K,
		'N': N,
		'c': c,
		'etas': etas,
		'delta': delta
	}

probs = {}

for result_file in RESULT_DIR.glob('trials*.csv'):
	configs = parse_result_file(result_file)

	# decoding probability varying eta for given configuration
	p = pd.read_csv(result_file, header=None)

	p.columns = ['c = {}'.format(configs['c'])]
	p = p.set_index(configs['etas'])

	# note that double {{}} excapes format
	tag = 'K={}, N={}, \delta={}'.format(configs["K"], configs["N"], configs["delta"])

	# add to probabilities dict or merge with existing one
	if tag in probs:
		probs[tag] = pd.concat([probs[tag], p], axis=1)
	else:
		probs[tag] = p

# plot decoding probability vs decoding ratio for various c, for each K, N, delta configuration
for config, scores in probs.items():
	fig = plt.figure(config)
	scores.plot(title='\\textrm{{ {} }}'.format(config), ax=fig.gca())
	plt.show()


# output the c with the maximum Area Under the Curve
# for decoding probability
for config, scores in probs.items():
	print("Optimal value for {} is {}".format(
		config,
		scores.sum(axis=0).idxmax()))
