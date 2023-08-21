# Scenario File

No matter what [run environment](environments.md) is chosen when running DAISI, all environments use a scenario file with some fields that are always required.

| Parameter | Default value | Required | Description |
|---|---|:-:|---|
| `title:` |  `MINHTON_default` | :fontawesome-solid-star-of-life: | Can be any string, purely descriptive at the moment |
| `name:` |  `MO test with fanout 2` | | Can be any string, purely descriptive at the moment |
| `version:` | `0.1` | :fontawesome-solid-star-of-life: | string, can be used to differentiate different implementation versions |
| `stoptime:` | `10000000` | :fontawesome-solid-star-of-life: | uint64_t, defines at which simulation time in milliseconds the simulation will be aborted if it does not finish earlier |
| `physicalLayer:` | `ETH` | :fontawesome-solid-star-of-life: | Currently unused |
| `randomSeed:` | `1` | :fontawesome-solid-star-of-life: | uint64_t, 0 = random, used for reproducible simulations |
| `defaultDelay:` | `5000` | :fontawesome-solid-star-of-life: | Delay between each simulation step (operations defined in the `scenarioSequence`, respectively between each step for *x*-many operations) in milliseconds. |
| `logLevel:` | `info` | | `debug`, `info`, `warning`, or `critical` |
| `outputPath:` | `/work/ns3/results/` | | Needs to be defined if the equivalent environment variable `DAISI_OUTPUT_PATH` is not set. This yml paramater takes precedence if both are set. |

```yaml
# required
title: Title
version: 0.1
stoptime: 10000000
physicalLayer: ETH
randomSeed: 1
defaultDelay: 5000

# optional
logLevel: info
outputPath: /work/ns3/results/

# required
scenarioSequence:
    ...
```

With `physicalLayer` we define the protocol for connecting the nodes to each other, by default we use Ethernet (`ETH`), but keep in mind that some simulation parts are unaffected from this setting and use Wi-Fi.

Below those mentioned fields the `scenarioSequence` is required, but what it can contain differs based on the run environment. Please refer to the specific components for details about the `scenarioSequence` ([MINHTON](../daisi_lib/minhton-ns3/management-overlay.md), [natter](../daisi_lib/natter-ns3/event-distribution.md), [CPPS](../daisi_lib/applications/cpps/index.md), [Path Planning](../daisi_lib/applications/path-planning/index.md)).
