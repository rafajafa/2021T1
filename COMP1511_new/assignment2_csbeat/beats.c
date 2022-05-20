// Assignment 2 21T1 COMP1511: Beats by CSE
// beats.c
//
// This program was written by Raphael Woo (z5360132)
// on 9/4/2021 to 22/4/2021
//
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.

#include "beats.h"

// Add your own #defines here.

//////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    struct beat *head;
    struct beat *curr;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    // You may choose to add or change fields in this struct.
    struct note *notes;
    struct beat *next;
};

// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};

// Add any other structs you define here.
typedef struct note* Note;

//////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.

// create a new_note with octave and key and a pointer pointing new_next
// return new_note
Note create_note(int octave, int key, Note new_next) {
    Note new_note = malloc(sizeof (struct note));
    new_note->octave = octave;
    new_note->key = key;
    new_note->next = new_next;
    return new_note;
}

// loop thorugh the note and see if the input octave and key is the highest
// return NOT_HIGHEST_NOTE or 0
int check_highest_note(Beat beat, int octave, int key) {
    int check_note = 0;
    Note curr = beat->notes;
    while (curr != NULL) {
        if (curr->octave > octave) {
            //if the input octave is smaller than octave before
            check_note = NOT_HIGHEST_NOTE;
        } else if (curr->octave == octave && curr->key >= key) {
            //if octave is the same and input key is smaller 
            check_note = NOT_HIGHEST_NOTE;
        }
        curr = curr->next;
    }
    return check_note;
}

// loop through the beat in track and return the number of beat in track
int count_beats_in_track(Track track) {
    int beat_number = 0;
    Beat curr = track->head;
    while (curr != NULL) {
        beat_number++;
        curr = curr->next;
    }
    return beat_number;
}

// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    return new_beat;
}

// a recusion function that would free all the notes in the beat
void free_note(Note head) {
    if (head == NULL) {
        // if there is no node in the beat
        return;
    } else {
        // otherwise use recursion to free other notes in the beat
        free_note(head->next);
        free(head);
    }
}

// a recusion function that would free all the beat in the track
void free_track_beat(Beat beat) {
    if (beat == NULL) {
        // if there is only no beat in the track
        return;
    } else {
        // else use recursion to free other beats in the btrack
        free_track_beat(beat->next);
        free_beat(beat);
    }
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

//////////////////////////////////////////////////////////////////////
//                        Stage 1 Functions                         //
//////////////////////////////////////////////////////////////////////

// Add a note to the end of a beat.
// check whether the note is valid first
// then create a node with all the info in it and put it at the end of beat 
int add_note_to_beat(Beat beat, int octave, int key) {
    int valid_note = 0;
    if (octave < 0 || octave >= 10) {
        // if octave is invalid
        valid_note = INVALID_OCTAVE;
    } else if (key < 0 || key >= 12) {
        // if key is invalid
        valid_note = INVALID_KEY;
    } else if (check_highest_note(beat, octave, key) == NOT_HIGHEST_NOTE) {
        // if not highest note
        valid_note = NOT_HIGHEST_NOTE;
    } else {
        if (beat->notes == NULL) {
            // if the beat is empty, create the first note
            Note new_note = create_note(octave, key, NULL);
            beat->notes = new_note;
        } else {
            // else loop through all the previous note and add the new note
            // to the end of it
            Note curr = beat->notes;
            Note prev = NULL;
            while (curr != NULL) {
                prev = curr;
                curr = curr->next;
            }
            Note new_note = create_note(octave, key, NULL);
            prev->next = new_note;
        }       
    }       
    return valid_note;
}

// Print the contents of a beat.
void print_beat(Beat beat) {
    int i = 0;
    Note curr = beat->notes;
    if (curr == NULL) {
        printf("\n");
    } else {
        while (curr != NULL) {
            //loop through the note and print out all of it
            if (i == 0) {
                // if it is the first note print
                printf("%d %02d ", curr->octave, curr->key);
                curr = curr->next;
                i++;
            } else {
                printf("| %d %02d ", curr->octave, curr->key);
                curr = curr->next;
                i++;
            }
        }
        printf("\n");    
    }
}

// Count the number of notes in a beat that are in a given octave.
// loop through the loop and when octave matches the given octave, num_note++ 
int count_notes_in_octave(Beat beat, int octave) {
    int num_note = 0;
    Note curr = beat->notes;
    while (curr != NULL) {
        if (curr->octave == octave) {
            //the octave in that note is equal to given octave
            num_note++;          
        }
        curr = curr->next;
    }   
    return num_note;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 2 Functions                         //
//////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    
    Track new_track = malloc(sizeof (struct track));
    new_track->curr = NULL;
    new_track->head = NULL;
    
    return new_track;
}

// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {
    Beat new_beat = beat;
    if (track->curr == NULL) {
        // if added as the first beat     
        if (track->head != NULL) {
            // if there is existed beat located at head
            // make it so that the new beat is at the top
            new_beat->next = track->head;
            track->head = new_beat;
        } else {
            // if there is no existed beat
            track->head = new_beat;
        }       
    } else {
        // if it is added in the middle or the end of the track 
        if (track->curr->next != NULL) {
            // if in the middle of track
            new_beat->next = track->curr->next;
            track->curr->next = new_beat;
        } else {
            //if in the end of track
            new_beat->next = NULL;
            track->curr->next = new_beat;         
        }       
    }
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {
    
    if (track->curr == NULL) {
        // if curr pointer is pointing at NULL (at the end or empty list)
        if (track->head == NULL) {
            // if the list is empty
            return TRACK_STOPPED;
        } else {
            // if the curr pointer is pointing at NULL     
            // and the list is not empty
            // so it will be the end of track
            // point to the head of the track
            track->curr = track->head;
            return TRACK_PLAYING;
        }
    } else {
        if (track->curr->next == NULL) {
            // if it is at the last beat
            track->curr = track->curr->next;
            return TRACK_STOPPED;
        } else {
            // otherwise point at the next beat
            track->curr = track->curr->next;
            return TRACK_PLAYING;
        }      
    }
}

// Print the contents of a track.
void print_track(Track track) {
    Beat curr = track->head;
    
    int num_beat = 0;
    while (curr != NULL) {
        // while the beat dont point to NULL, print all the note in that beat 
        // and move on to next beat       
        num_beat++;
        if (track->curr == curr) {
            //if this is the beat is being constructing
            printf(">");
        } else {
            printf(" ");
        }
        printf("[%d] ", num_beat);        
        print_beat(curr);         
        curr = curr->next;
    }
}

// Count beats after the current beat in a track.
// loop through all the beat and return the number 
int count_beats_left_in_track(Track track) {
    int beat_number = 0;
    if (track->curr == NULL) {
        // if curr is point at NULL
        // number of beat remind will = to all the beat in track
        beat_number = count_beats_in_track(track);
    } else {
        Beat curr_next = track->curr->next;
        while (curr_next != NULL) {
            // loop through the track starting from the 'track->curr->next' 
            // pointer
            // +1 after every loop
            beat_number++;
            curr_next = curr_next->next;
        }
    }
    return beat_number;
}

//////////////////////////////////////////////////////////////////////
//                        Stage 3 Functions                         //
//////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    
    if (beat->notes == NULL) {
        // if beat dont have any note in it
        // free the beat
        free(beat);
    } else {
        // give pointer of the first note to a recursion function that will
        // free all the notes
        // then run the free_beat again so that it will free the beat itself
        free_note(beat->notes);
        free(beat);
    }
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    
    if (track->head == NULL) {
        // if there is no beat in the track
        free(track);
    } else {
        // if there is multiple beat in track
        // give the pointer of first beat to a recursion function and free 
        // the beats
        free_track_beat(track->head);
        free(track);
    }
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    
    Beat prev = track->head;
    Beat temp = NULL;
    
    if (track->curr != NULL) {
        // if user is pointing a beat in the track
        if (prev == track->curr) {
            // if user want to delete the first note
            temp = track->curr;
            track->head = track->curr->next;
            track->curr = track->curr->next;
            free_beat(temp);
            if (track->curr == NULL) {
                // if the deleted node is the only node in track
                // return TRACK_STOPPED 
                return TRACK_STOPPED;
            } else {
                return TRACK_PLAYING;
            } 
        } else {
            // if a beat is being selected currently is not the first beat
            while (prev->next != track->curr) {               
                // loop through the track to find the one beat before the 
                // selected beat
                prev = prev->next;
            }
            // link the previous node to the curr->next node then free 
            // the curr node 
            temp = track->curr;
            prev->next = track->curr->next;
            track->curr = track->curr->next;
            free_beat(temp);
            if (track->curr == NULL) {
                // if the deleted node is at the end of the list
                // return TRACK_STOPPED 
                return TRACK_STOPPED;
            } else {
                return TRACK_PLAYING;
            } 
        }             
    } else {
        return TRACK_STOPPED;
    } 
}



