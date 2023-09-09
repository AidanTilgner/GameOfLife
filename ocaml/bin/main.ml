open Graphics

type game_state = {
  frame: int;
}

let rec game_loop game_state last_frame_time =
  (* Calculate delta time *)
  let current_time = Unix.gettimeofday () in
  let delta_time = current_time -. last_frame_time in

  (* Update game state *)
  let new_game_state = {
    frame = game_state.frame + 1;
  } in

  (* Print or use delta_time and new_game_state for debugging or logic *)
  Printf.printf "Frame: %d, Delta Time: %f\n" new_game_state.frame delta_time;

  (* Recursive call to continue the game loop *)
  game_loop new_game_state current_time  (* Note: passing current_time, not delta_time *)

let initial_game_state : game_state = {
  frame = 0
}

let () =
  open_graph " 1000x500";
  set_window_title "Game of Life";

  game_loop initial_game_state 0.;

  let e = wait_next_event [Button_down] in
  if e.button then close_graph ()
