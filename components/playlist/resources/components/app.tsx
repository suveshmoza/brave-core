/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { bindActionCreators, Dispatch } from 'redux'
import { connect } from 'react-redux'

import { Playlist } from 'components/definitions/playlist'

// Components for playlist
import { CloseCircleOIcon } from 'brave-ui/components/icons'
import Table, { Cell, Row } from 'brave-ui/components/dataTables/table'
import PlaylistItem from './playlistItem'

// Utils
import * as playlistActions from '../actions/playlist_action_creators'
import { getPlaylistAPI } from '../api/api'

import * as PlaylistMojo from 'gen/brave/components/playlist/mojom/playlist.mojom.m.js'

interface Props {
  actions: any
  playlistData: Playlist.State
}

interface State {
  playlist?: PlaylistMojo.Playlist
  experimentalUrl: string
}

export class PlaylistPage extends React.Component<Props, State> {
  constructor (props: Props) {
    super(props)
    this.state = { experimentalUrl: '' }
    this.onClickDownloadVideo = this.onClickDownloadVideo.bind(this)
  }

  get actions () {
    return this.props.actions
  }

  getCurrentPlaylist = () => {
    // TODO(sko): get current playlist index from store
    return this.props.playlistData.lists.at(0)
  }

  getImgSrc = (itemId: string) => {
    return 'chrome://playlist-data/' + itemId + '/thumbnail/'
  }

  getMediaSrc = (itemId: string) => {
    return 'chrome://playlist-data/' + itemId + '/media'
  }

  get lazyButtonStyle () {
    const lazyButtonStyle: any = {
      alignItems: 'center',
      WebkitAppearance: 'none',
      width: '50px',
      height: '50px',
      display: 'flex',
      borderRadius: '4px'
    }
    return lazyButtonStyle
  }

  getPlaylistHeader = (): Cell[] => {
    return [
      { content: 'INDEX' },
      { content: 'NAME' },
      { content: 'STATUS' },
      { content: 'REMOVE' }
    ]
  }

  getPlaylistRows = (playlist?: any): Row[] | undefined => {
    if (!playlist || !playlist.items) {
      return
    }

    return playlist.items.map((item: PlaylistMojo.PlaylistItem, index: any): any => {
      const cell: Row = {
        content: [
          { content: (<div style={{ textAlign: 'center' }}>{index + 1}</div>) },
          {
            content: (
                <PlaylistItem id={item.id} name={item.name} onClick={this.onClickItem.bind(this)}
                    thumbnailUrl={item.thumbnailPath.url.startsWith('http')
                        ? '' // TODO(sko): currently, requesting for other host isn't allowed for this page
                        : this.getImgSrc(item.id)}/>
            )
          },
          { content: (<span>{item.ready ? 'Ready' : 'Downloading'}</span>) },
          { content: (<button style={this.lazyButtonStyle} onClick={this.onClickRemoveItemButton.bind(this, item.id)}><CloseCircleOIcon /></button>) }
        ]
      }
      return cell
    })
  }

  onClickRemoveItemButton = (playlistItemId: string) => {
    getPlaylistAPI().removeItemFromPlaylist('', playlistItemId)
  }

  get pageHasDownloadableVideo () {
    return this.state.experimentalUrl.startsWith('https://www.youtube.com/watch')
  }

  onChangeExperimentalUrl = (event: React.ChangeEvent<HTMLTextAreaElement>) => {
    this.setState({ experimentalUrl: event.target.value })
  }

  onClickDownloadVideo = () => {
    getPlaylistAPI().addMediaFilesFromPageToPlaylist('', this.state.experimentalUrl)
  }

  onClickItem = (itemId: string) => {
    const item = this.getCurrentPlaylist()?.items.find((item: PlaylistMojo.PlaylistItem) => {
      return item.id === itemId
    })

    if (!item || !item.ready) {
      return
    }

    document.getElementById('player')?.setAttribute('src', this.getMediaSrc(itemId))
  }

  render () {
    return (
      <div id='playlistPage'>
        <div style={{ minHeight: '600px', width: '1200px' }}>
          <Table header={this.getPlaylistHeader()} rows={this.getPlaylistRows(this.getCurrentPlaylist())}>
            YOUR PLAYLIST IS EMPTY
          </Table>
        </div>

        <video id="player" controls autoPlay />

        <div>
          <h1>Experimental</h1>
          <div>
            <textarea
              cols={100}
              rows={10}
              value={this.state.experimentalUrl}
              onChange={this.onChangeExperimentalUrl}
            />
            <div>
            {
              this.pageHasDownloadableVideo
              ? (
                <div>
                  <h1>This page has a video you can download</h1>
                  <button onClick={this.onClickDownloadVideo}>Click here to download</button>
                </div>
              ) : (
                <h1>Nothing to see here. Put a proper YouTube link to see the magic</h1>
              )
            }
            </div>
          </div>
        </div>
      </div>
    )
  }
}

export const mapStateToProps = (state: Playlist.ApplicationState) => ({
  playlistData: state.playlistData
})

export const mapDispatchToProps = (dispatch: Dispatch) => ({
  actions: bindActionCreators(playlistActions, dispatch)
})

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(PlaylistPage)
