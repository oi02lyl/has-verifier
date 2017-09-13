# has-verifier
Implementation of two Verifiers for Hierarchical Artifact Systems
1. Spin-based [link](https://arxiv.org/abs/1705.09427)
2. Karp-Miller-based (VERIFAS) [link](https://arxiv.org/abs/1705.10007)
2. VLDB PhD workshop paper [link](http://ceur-ws.org/Vol-1882/paper14.pdf)

## Prerequisite
* Boost
* Flex + Bison
* [g++-6](https://gcc.gnu.org/gcc-6/)
* [Spin](http://spinroot.com/spin/Bin/)

## How to compile
Simply use the Makefile in verifier/

## How to run
`./has_real ../bpmn/INPUT_FILE_NAME MODE`
* Spin-based: MODE = 1
* Karp-Miller-based: MODE = 0, or MODE = 2 to ignore the artifact relations

The verifier generates and verifier a simple liveness property

## How to run batch experiment
#### Real set of workflows
* Spin: `python run_spin.py`
* Karp-Miller: `python run_km.py`
* Karp-Miller with no artifact relations: `python run_km_noset.py`

#### Synthetic set of workflows
* Spin: `python run_spin_synthetic.py`
* Karp-Miller: `python run_km_synthetic.py`
* Karp-Miller with no artifact relations: `python run_km_noset_synthetic.py`
