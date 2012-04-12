/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
 *   trekkie@media-mission.de                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mixerchannel_fileplayer.h"
#include "mixerchannelmanager.h"

QString const mixerChannel_filePlayer::Type = QString("PLAYER");

mixerChannel_filePlayer::mixerChannel_filePlayer( QString name, QUuid uuid )
 : mixerChannel(name, uuid)
{
    type = Type;
    bufferThread = new channelBufMngr(this);
    fileOpen = FALSE;
    loopMode = FALSE;
    decoder = NULL;
    playlist = NULL;

    registerChannel();
}

mixerChannel_filePlayer::~mixerChannel_filePlayer()
{
    // unregister myself in the channel manager
    mixerChannelManager::unregisterChannel( this );

    if(!state == Stopped)
        stop();
    bufferThread->terminate();
}

void mixerChannel_filePlayer::open( playListItem* track )
{
	meta = track;

	soundBuffers[0].flush();
	soundBuffers[1].flush();

	if(fileOpen)
	    stop();

        fHandle = fopen( track->getFile().toAscii(), "r");
	if( !fHandle )
	{
                qWarning( "%s", tr("file open error.. proberly not found..").append(track->getFile()).toAscii().data());
		return;
	}
	fileName = track->getFile();

	try
	{
		QRegExp rx( "^(.*)\\.(\\w+)$" );
		if ( rx.indexIn( track->getFilename() ) != -1 ) {
#ifdef HAVE_OGG
			if( rx.cap(2).toLower() == "ogg" )
			{
				oggDecoder::readMetaFromFile( track );
                                decoder = new oggDecoder( fHandle, this );
			}
#endif
#ifdef HAVE_MAD
			if( rx.cap(2).toLower() == "mp3" )
			{
				mpgDecoder::readMetaFromFile( track );
                                decoder = new mpgDecoder( fHandle, this );
			}
#endif
			if( decoder == NULL )
			{
				QMessageBox::warning( NULL, tr("RadioMixer - Playerchannel File"), tr("unknown Filetype: ")+rx.cap(2).toLower(), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
				fileOpen = FALSE;
			}else{
//				decoder->setMetaInfos( &meta );
                                smplRate = track->getSamplerate();
				channels = track->getChannels();
				track->setLength( QTime(0,0,0,0 ).addMSecs( decoder->getTotalFrames()*40 ) );

				fileOpen = TRUE;
				if( meta )
					dynamic_cast<playListItem*>(meta)->cueing();
                                emit( cued( track ) );
                                setState( Cued );

				if(!bufferThread->isRunning())
					bufferThread->start();
				emit( refreshed() );
			}
		}
	}
	catch( decoderException* ex )
	{
		qWarning( "%s", ex->msg.toAscii().data() );
		fileOpen = FALSE;
	}
}

void mixerChannel_filePlayer::attachToPlaylist( playList* playlist )
{
    this->playlist = playlist;
}

void mixerChannel_filePlayer::detachPlaylist( )
{
    this->playlist = NULL;
}

void mixerChannel_filePlayer::decode( )
{
        bool endOfTrack = false;
	float** decBuff = new float*[2];		//Buffer for Sound Decoder
        int dataFetched = 0;
	unsigned int toFetch = soundBuffers[0].getFree();

        dataFetched = decoder->decode( &decBuff, toFetch );

	// Update track position
        if(position_change_counter >= 1)
        {
            emit(positionChanged( QTime(0,0,0,0).addMSecs(decoder->getPlayedFrames()*40) ));
            position_change_counter = 0;
        }else
            position_change_counter++;

        if(dataFetched == 0)
	{
                if( loopMode )
		{
			decoder->reset();
			dataFetched = decoder->decode( &decBuff, toFetch );
			if(dataFetched == 0)
				stop();
		}
		else
                {
                    stop();
                    endOfTrack = 1;
                }
	}else
	{
		// process Audio....
		//mix Monot to Stereo
		if( channels == 1 )
		{
                    //Write Data to ringbuffer mono mode...
                    soundBuffers[0].write( decBuff[0], dataFetched );
                    soundBuffers[1].write( decBuff[0], dataFetched );
                }else
                {
                    //Write Data to ringbuffer stereo mode..
                    soundBuffers[0].write( decBuff[0], dataFetched );
                    soundBuffers[1].write( decBuff[1], dataFetched );
                }
	}
	if( endOfTrack )
            this->endOfTrack();
}

void mixerChannel_filePlayer::endOfTrack()
{
    // first ensure the current file is closed after the track has ended..
    this->close();

    // auto recue from playlist ??
    if(settings["autoload_fromplaylist"].toBool())
        this->cue();

    // send end of track signal..
    emit( trackEnded() );
}

void mixerChannel_filePlayer::checkBuffer( )
{
        if( soundBuffers[0].canWrite( 1024 ) && state == Playing )
		decode();
}

void mixerChannel_filePlayer::stop( )
{
    if(bufferThread->isRunning())
	bufferThread->stop();
    if( state == Stopped )
        return;
    if( fileOpen )
        dynamic_cast<playListItem*>(meta)->stopped();
    close();

    soundBuffers[0].flush();
    soundBuffers[1].flush();
    setState( Stopped );
}

void mixerChannel_filePlayer::close( )
{
	if(fileOpen)
	{
		delete decoder;
		decoder = NULL;
		fileOpen = FALSE;
	}
}

void mixerChannel_filePlayer::setState(playerState newState )
{
    this->state = newState;
    emit(stateChanged(state));
}

void mixerChannel_filePlayer::cue( )
{
    if(playlist != NULL)
        this->open( playlist->getNext() );
}

const bool mixerChannel_filePlayer::isFileOpen( )
{
	return fileOpen;
}

const int mixerChannel_filePlayer::getRTime( )
{
	return decoder->getRTime();
}

const int mixerChannel_filePlayer::getTime( )
{
	return decoder->getTime();
}

void mixerChannel_filePlayer::setName( QString newName )
{
        mixerChannel::setName( newName );
	emit( refreshed() );
}

void mixerChannel_filePlayer::play( )
{
    if( !fileOpen || state == Playing )
        return;
    if( fileOpen )
        dynamic_cast<playListItem*>(meta)->startPlaying();
    setState( Playing );
    if( !settings["ignore_metadata"].toBool() )
        emit( newMeta( *meta ) );
}

const float mixerChannel_filePlayer::getPosition_Samples( )
{
	return decoder->getPosition_Samples();
}

const float mixerChannel_filePlayer::getTotal_Samples( )
{
	return decoder->getTotal_Samples();
}

const float mixerChannel_filePlayer::getTotalFrames( )
{
	return decoder->getTotalFrames();
}

const float mixerChannel_filePlayer::getPlayedFrames( )
{
	return decoder->getPlayedFrames();
}

const float mixerChannel_filePlayer::getRemainFrames( )
{
	return decoder->getRemainFrames();
}

void mixerChannel_filePlayer::pause( )
{
    // pausing if we are not in Play mode makes no sence............
    if( state != Playing )
        return;
    setState( Paused );
    // reset buffers
    soundBuffers[0].flush();
    soundBuffers[1].flush();
    // reset vuMeter by resetting last levels
    emit( vuMeterChanged_left(0) );
    emit( vuMeterChanged_right(0) );
}

const bool mixerChannel_filePlayer::isLooping( )
{
	return loopMode;
}

void mixerChannel_filePlayer::toggleLoop( )
{
        loopMode = !loopMode;
        emit(loopChanged( loopMode ));
}

const float mixerChannel_filePlayer::getPrerollFrames( )
{
	return meta->getPreLength().second()*25;
}

bool mixerChannel_filePlayer::hasAutomationSupport()
{
    return true;
}
