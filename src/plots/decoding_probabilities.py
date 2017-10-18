import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib import rc

rc('text', usetex=True)

RESULT_DIR = Path('../../results/simulator')

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

	p.columns = ['{}, {}'.format(configs['problem'], configs['solutor'])]
	p = p.set_index(np.linspace(1, 2.5, 10))

	# note that double {{}} excapes format
	tag = 'K={}, N={}, \delta={}'.format(
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


# plot decoding probability vs decoding ratio for various c, for each K, N, delta configuration
for config, scores in probs.items():
	fig = plt.figure(config)
	scores.plot(title='\\textrm{{ {} }}'.format(config), ax=fig.gca())
	plt.show()
