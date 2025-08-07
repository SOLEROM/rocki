# enable


For users of RK356X products, you need to enable the NPU in the terminal using rsetup before using the NPU: sudo rsetup -> Overlays -> Manage overlays -> Enable NPU, then restart the system.

If there is no Enable NPU option in the overlays options, please update the system via: sudo rsetup -> system -> System Update, restart, and execute the above steps to enable the NPU.