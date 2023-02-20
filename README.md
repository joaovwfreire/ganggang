On contract documentation:
    Function name
    Description
    Parameters
    Return value
    Example
    Notes

# NonFungibleGang DAO Factory Smart Contract (NFGDAOFactory)

## Overview
Non-Fungible Gang's DAO Factory for creating new DAOs using the AtomicAssets non-fungible token standard. This contract works on EOS and it's forks(like Telos, Worbli, etc) - and of course, WAX.
It is an easy way to create a DAO with a custom Non-Fungible Token (NFT) asset and distribute Fungible Tokens (FT) to the DAO members.
The DAO members can vote on proposals and the proposals can be executed by the DAO members.
The DAO governors can also create new proposals and execute them.
The DAO main account is the account that created the DAO and we intend to create a guide on how to make this account a multisig account.

New members can join the DAO by buying the DAO NFT asset from the AtomicMarketplace and staking the NFT asset to the DAO main account by accessing
the DAO dashboard at dao.nonfungiblegang.com.


## Table of Contents
### 1. [Actions](#1-actions)
1.1 [Main](#11-main) |
1.2 [Payment](#12-payment) |
1.3 [Governance](#13-governance) |
1.4 [Proposals](#14-proposals) |
1.5 [Staking](#15-staking) 
### 2. [Tables](#2-tables)
2.1 [custodians](#21-custodians) |
2.2 [candidates](#22-candidates) |
2.3 [membership](#23-membership) |
2.4 [memconfigs](#24-memconfigs) |
2.5 [totalweights](#25-totalweights) |
2.6 [proposals](#26-proposals) |
2.7 [propvotes](#27-propvotes) |
2.8 [elections](#28-elections) |
2.9 [daos_p](#29-daos_p) |
2.10 [daotokens](#210-daotokens) |
2.11 [receipts](#211-receipts) 



## 1. Actions

### 1.1 Main
#### 1.1.1 `registerdao`
##### Description
Register a new DAO
##### Ricardian Contract
The `registerdao` action registers a new DAO.
`registerdao` registers a new DAO with the following parameters:
- `dao_name` - The name of the DAO
- `dao_symbol` - symbol code of the DAO's token
- `token_contract` - The contract of the DAO's token
- `dao_payer` - The account that pays for the DAO

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `dao_name` | name | The name of the DAO |
| `dao_symbol` | symbol_code | symbol code of the DAO's token |
| `token_contract` | name | The contract of the DAO's token |
| `dao_payer` | name | The account that pays for the DAO |


#### 1.1.2 `setweights`
##### Description
Set the weights of the DAO assets
##### Ricardian Contract
The `setweights` action sets the weights of the DAO assets.
`setweights` sets the weights of the DAO assets with the following parameters:
- `dao_name` - The name of the DAO
- `template_id` - The template id of the asset
- `vote_weight` - The weight of the asset for voting
- `share_weight` - The weight of the asset for shares

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `dao_name` | name | The name of the DAO |
| `template_id` | uint32_t | The template id of the asset |
| `vote_weight` | uint16_t | The weight of the asset for voting |
| `share_weight` | uint16_t | The weight of the asset for shares |


### 1.2 Payment
#### 1.2.1 `addpayment`
##### Description
Add a payment to the DAO
##### Ricardian Contract
The `addpayment` action adds a payment to the DAO.
`addpayment` adds a payment to the DAO with the following parameters:
- `dao_name` - The name of the DAO
- `quantity` - The quantity of the payment

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `dao_name` | name | The name of the DAO |
| `quantity` | asset | The quantity of the payment |


#### 1.2.2 `claimp`
##### Description
Claim a payment from the DAO
##### Ricardian Contract
The `claimp` action claims a payment from the DAO.
`claimp` claims a payment from the DAO with the following parameters:
- `user` - The user that claims the payment
- `dao_name` - The name of the DAO
- `staked_asset` - The asset id of the staked asset

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `user` | name | The user that claims the payment |
| `dao_name` | name | The name of the DAO |
| `staked_asset` | uint64_t | The asset id of the staked asset |


### 1.3 Governance
#### 1.3.1 `addcandidate`
##### Description
Add a candidate to the DAO
##### Ricardian Contract
The `addcandidate` action adds a candidate to the DAO.
`addcandidate` adds a candidate to the DAO with the following parameters:
- `cand` - The candidate account
- `dao_name` - The name of the DAO  
- `asset_id` - The asset id of the staked asset

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `cand` | name | The candidate account |
| `dao_name` | name | The name of the DAO |
| `asset_id` | uint64_t | The asset id of the staked asset |

#### 1.3.2 `startelect`
##### Description
Start an election
##### Ricardian Contract
The `startelect` action starts an election.
`startelect` starts an election with the following parameters:
- `dao_name` - The name of the DAO
- `end` - The end time of the election
- `elected_n` - The number of elected candidates

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `dao_name` | name | The name of the DAO |
| `end` | uint64_t | The end time of the election |
| `elected_n` | uint8_t | The number of elected candidates |


#### 1.3.3 `finishelect`
##### Description
Finish an election
##### Ricardian Contract
The `finishelect` action finishes an election.
`finishelect` finishes an election with the following parameters:
- `dao_name` - The name of the DAO


### 1.4 Proposals
#### 1.4.1 `addproposal`
##### Description
Add a proposal to the DAO
##### Ricardian Contract
The `addproposal` action adds a proposal to the DAO.
`addproposal` adds a proposal to the DAO with the following parameters:
- `cust` - The account that proposes the proposal
- `dao_name` - The name of the DAO
- `asset_id` - The asset id of the staked asset
- `proposal` - The proposal

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `cust` | name | The account that proposes the proposal |
| `dao_name` | name | The name of the DAO |
| `asset_id` | uint64_t | The asset id of the staked asset |
| `proposal` | string | The proposal |

#### 1.4.2 `voteprop`
##### Description
Vote for a proposal
##### Ricardian Contract
The `voteprop` action votes for a proposal.
`voteprop` votes for a proposal with the following parameters:
- `voter` - The account that votes for the proposal
- `dao_name` - The name of the DAO
- `proposal_id` - The id of the proposal
- `vote` - The vote

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `voter` | name | The account that votes for the proposal |
| `dao_name` | name | The name of the DAO |
| `proposal_id` | uint64_t | The id of the proposal |
| `vote` | uint8_t | The vote |

### 1.5 Staking
#### 1.5.1 `stake`
##### Description
Stake an asset to the DAO
##### Ricardian Contract
The `stake` action stakes an asset to the DAO.
`stake` stakes an asset to the DAO with the following parameters:
- `user` - The account that stakes the asset
- `dao_name` - The name of the DAO
- `quantity` - The quantity of the asset
- `asset_id` - The asset id of the staked asset

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `user` | name | The account that stakes the asset |
| `dao_name` | name | The name of the DAO |
| `quantity` | asset | The quantity of the asset |
| `asset_id` | uint64_t | The asset id of the staked asset |

#### 1.5.2 `unstake`
##### Description
Unstake an asset from the DAO
##### Ricardian Contract
The `unstake` action unstakes an asset from the DAO.
`unstake` unstakes an asset from the DAO with the following parameters:
- `user` - The account that unstakes the asset
- `dao_name` - The name of the DAO
- `quantity` - The quantity of the asset
- `asset_id` - The asset id of the staked asset

##### Parameters
| Parameter | Type | Description |
| --- | --- | --- |
| `user` | name | The account that unstakes the asset |
| `dao_name` | name | The name of the DAO |
| `quantity` | asset | The quantity of the asset |
| `asset_id` | uint64_t | The asset id of the staked asset |


## 2. Tables

### 2.1 Custodians

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `cust_name` | name | The name of the custodian | Primary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `total_votes` | uint64_t | The total votes of the custodian | |
| `cust_end` | uint64_t | The end time of the custodian | |

### 2.2 Candidates

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `cand_id` | uint64_t | The id of the candidate | Primary Key |
| `cand_name` | name | The name of the candidate | Secondary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `votes` | uint64_t | The votes of the candidate | Secondary Key |
| `election_id` | uint64_t | The id of the election | Secondary Key |

### 2.3 Membership

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `asset_id` | uint64_t | The asset id of the staked asset | Primary Key |
| `user` | name | The account that stakes the asset | Secondary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `shares` | uint32_t | The share power of the staked asset | |
| `votes` | uint32_t | The voting power of the staked asset | |
| `reg_time` | uint64_t | The registration time of the staked asset | |
| `l_p_claim` | uint64_t | The last time the staked asset claimed payment | |
| `l_e_vote` | uint64_t | The last time the staked asset voted for an election | |

### 2.4 Memconfigs

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `template_id` | uint32_t | The id of the template | Primary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `vote_weight` | uint32_t | The voting weight of the template | |
| `share_weight` | uint32_t | The share weight of the template | |

### 2.5 Totalweights

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `dao_name` | name | The name of the DAO | Primary Key |
| `vote_totals` | uint32_t | The total voting power of the DAO | |
| `share_totals` | uint32_t | The total share power of the DAO | |

### 2.6 Proposals

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `proposal_id` | uint64_t | The id of the proposal | Primary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `custodian` | name | The name of the custodian | Secondary Key |
| `proposal` | string | The proposal | |
| `votes_for` | uint64_t | The votes for the proposal | |
| `votes_against` | uint64_t | The votes against the proposal | |
| `approved` | uint8_t | The approval status of the proposal | |
| `expiry` | uint64_t | The expiry time of the proposal | |

### 2.7 Propvotes

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `id` | uint64_t | The id of the proposal vote | Primary Key |
| `proposal_id` | uint64_t | The id of the proposal | Secondary Key |
| `user` | name | The account that votes for the proposal | Secondary Key |
| `vote` | uint8_t | The vote of the account | |

### 2.8 Elections

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `election_id` | uint64_t | The id of the election | Primary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `start` | uint64_t | The start time of the election | |
| `end` | uint64_t | The end time of the election | |
| `elected_n` | uint8_t | The number of elected custodians | |
| `votes_issued` | uint64_t | The total votes already issued at the election | |
| `total_votes` | uint64_t | The total possible votes of the election | |


### 2.9 Daos_p

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `payment_id` | uint64_t | The id of the payment | Primary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `total_shares` | uint64_t | The total shares of the DAO at the moment the payment was issued | |
| `quantity` | asset | The quantity of the payment | |

### 2.10 Daotokens

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `dao_name` | name | The name of the DAO | Primary Key |
| `token_contract` | name | The contract of the token | |
| `token_symbol` | symbol_code | The symbol of the token | |
| `dao_payer` | name | The account that handles DAO's payments | |

### 2.11 Receipts

| Parameter | Type | Description | Index |
| --- | --- | --- | --- |
| `receipt_id` | uint64_t | The id of the receipt | Primary Key |
| `payment_id` | uint64_t | The id of the payment | Secondary Key |
| `dao_name` | name | The name of the DAO | Secondary Key |
| `user` | name | The account that receives the payment | Secondary Key |
| `quantity` | asset | The quantity of the payment | |








