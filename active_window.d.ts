/// <reference types="node" />

declare namespace ActiveWindow {
    interface WindowMetadata {
        title: string,
        process_id: number,
        process_path: string
    }

    function getActiveWindow(): WindowMetadata;
}

declare module 'active_window' {
    export = ActiveWindow;
}
