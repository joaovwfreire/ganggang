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
### 3. [Scopes](#3-scopes)
### 4. [Ricardian Contracts](#4-ricardian-contracts)
### 5. [Multi Index Table Definitions](#5-multi-index-table-definitions)

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



