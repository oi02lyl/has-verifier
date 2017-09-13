# has-verifier
Implementation of two Verifiers for Hierarchical Artifact Systems
1. Spin-based [link](https://arxiv.org/abs/1705.09427)
2. Karp-Miller-based (VERIFAS) [link](https://arxiv.org/abs/1705.10007)
3. VLDB PhD workshop paper [link](http://ceur-ws.org/Vol-1882/paper14.pdf)
4. More on the theory [link](https://arxiv.org/abs/1604.00967)

## Prerequisite
* Boost
* Flex + Bison
* [g++-6](https://gcc.gnu.org/gcc-6/)
* [Spin](http://spinroot.com/spin/Bin/)
* Python 2.7 (for running the experiment scripts)

## How to compile
Simply use the Makefile in verifier/

## How to run
`./has_real ../bpmn/INPUT_FILE_NAME MODE NAIVE PROPERTY`
* Mode: 
  * 0 -> VERIFAS, the karp-miller based verifier
  * 1 -> Spin-based verifier
  * 2 -> VERIFAS with artifact relations ignored
* NAIVE:
  * 0 -> All opimizations are on
  * 1-4 (1-2 for Spin) -> One of the opimizations is turned off
* PROPERTY: 0 - 11, template of the LTL-FO property to be verified. Please refer to Table 4 of [link](https://arxiv.org/abs/1705.10007) for more details. A LTL-FO property will be generated from the template and verified by the verifier.

## How to run batch experiment
#### Real set of workflows
* Spin: `python run_spin.py`
* Karp-Miller: `python run_km.py`
* Karp-Miller with no artifact relations: `python run_km_noset.py`

#### Synthetic set of workflows
* Spin: `python run_spin_synthetic.py`
* Karp-Miller: `python run_km_synthetic.py`
* Karp-Miller with no artifact relations: `python run_km_noset_synthetic.py`
