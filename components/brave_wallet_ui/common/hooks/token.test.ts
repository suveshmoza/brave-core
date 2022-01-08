import { renderHook, act } from '@testing-library/react-hooks'
import {
  mockERC20Token,
  mockNetwork
} from '../constants/mocks'
import { FindBlockchainTokenInfoReturnInfo } from '../../constants/types'
import useTokenInfo from './token'

const findBlockchainTokenInfo = async (address: string) => {
  return await { token: null } as FindBlockchainTokenInfoReturnInfo
}

describe('useTokenInfo hook', () => {
  it('Should return DOG token info from visibleTokens list', () => {
    const { result } = renderHook(() => useTokenInfo(
      findBlockchainTokenInfo, [mockERC20Token], [], mockNetwork
    ))
    act(() => result.current.onFindTokenInfoByContractAddress('mockContractAddress'))
    expect(result.current.foundTokenInfoByContractAddress?.name).toEqual('Dog Coin')
  })

  it('Should return DOG token info from fullTokens list', () => {
    const { result } = renderHook(() => useTokenInfo(
      findBlockchainTokenInfo, [], [mockERC20Token], mockNetwork
    ))
    act(() => result.current.onFindTokenInfoByContractAddress('mockContractAddress'))
    expect(result.current.foundTokenInfoByContractAddress?.name).toEqual('Dog Coin')
  })

  it('Should not find info and return undifined', () => {
    const { result } = renderHook(() => useTokenInfo(
      findBlockchainTokenInfo, [mockERC20Token], [], mockNetwork
    ))
    act(() => result.current.onFindTokenInfoByContractAddress('testContractAddress'))
    expect(result.current.foundTokenInfoByContractAddress?.name).toEqual(undefined)
  })
})
